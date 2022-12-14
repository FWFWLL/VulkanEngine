#include "Model.hpp"
#include "Utils.hpp"

// Libraries
#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan_core.h>

// STD
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <unordered_map>

#ifndef ENGINE_DIR
#define ENGINE_DIR "../"
#endif

namespace std {

template<>
struct hash<FFL::Model::Vertex> {
	size_t operator()(FFL::Model::Vertex const& p_vertex) const {
		size_t seed = 0;

		FFL::hashCombine(seed, p_vertex.position, p_vertex.color, p_vertex.normal, p_vertex.uv);

		return seed;
	}
};

}

namespace FFL {

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {};

	attributeDescriptions.push_back({0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
	attributeDescriptions.push_back({1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
	attributeDescriptions.push_back({2, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
	attributeDescriptions.push_back({3, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

	return attributeDescriptions;
}

void Model::Builder::loadModel(const std::string& p_filePath) {
	std::string enginePath = ENGINE_DIR + p_filePath;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, enginePath.c_str())) {
		throw std::runtime_error(warn + err);
	}

	vertices.clear();
	indices.clear();

	std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
	for(const tinyobj::shape_t& shape : shapes) {
		for(const tinyobj::index_t& index : shape.mesh.indices) {
			Vertex vertex = {};

			if(index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};

				vertex.color = {
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2],
				};
			}

			if(index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}

			if(index.texcoord_index >= 0) {
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}

			if(uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}
}

Model::Model(Device& p_device, const Model::Builder& p_builder) : m_device{p_device} {
	createVertexBuffers(p_builder.vertices);
	createIndexBuffer(p_builder.indices);
}

Model::~Model() {}

std::unique_ptr<Model> Model::createModelFromFile(Device& p_device, const std::string& p_filePath) {
	Builder builder = {};
	builder.loadModel(p_filePath);

	std::cout << "Vertex Count: " << builder.vertices.size() << '\n';

	return std::make_unique<Model>(p_device, builder);
}

void Model::createVertexBuffers(const std::vector<Vertex>& p_vertices) {
	m_vertexCount = static_cast<uint32_t>(p_vertices.size());
	assert(m_vertexCount >= 3 && "Vertex count must be at least 3");

	VkDeviceSize bufferSize = sizeof(p_vertices[0]) * m_vertexCount;
	uint32_t vertexSize = sizeof(p_vertices[0]);

	Buffer stagingBuffer{m_device, vertexSize, m_vertexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

	stagingBuffer.map();
	stagingBuffer.writeToBuffer((void*)p_vertices.data());

	m_vertexBuffer = std::make_unique<Buffer>(m_device, vertexSize, m_vertexCount, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_device.copyBuffer(stagingBuffer.getBuffer(), m_vertexBuffer->getBuffer(), bufferSize);
}


void Model::createIndexBuffer(const std::vector<uint32_t>& p_indices) {
	m_indexCount = static_cast<uint32_t>(p_indices.size());
	m_hasIndexBuffer = m_indexCount > 0;

	if(!m_hasIndexBuffer) {
		return;
	}

	VkDeviceSize bufferSize = sizeof(p_indices[0]) * m_indexCount;
	uint32_t indexSize = sizeof(p_indices[0]);

	Buffer stagingBuffer{m_device, indexSize, m_indexCount, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};

	stagingBuffer.map();
	stagingBuffer.writeToBuffer((void*)p_indices.data());

	m_indexBuffer = std::make_unique<Buffer>(m_device, indexSize, m_indexCount, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	m_device.copyBuffer(stagingBuffer.getBuffer(), m_indexBuffer->getBuffer(), bufferSize);
}

void Model::bind(VkCommandBuffer p_commandBuffer) {
	VkBuffer buffers[] = {m_vertexBuffer->getBuffer()};
	VkDeviceSize offsets[] = {0};

	vkCmdBindVertexBuffers(p_commandBuffer, 0, 1, buffers, offsets);

	if(m_hasIndexBuffer) {
		vkCmdBindIndexBuffer(p_commandBuffer, m_indexBuffer->getBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}
}

void Model::draw(VkCommandBuffer p_commandBuffer) {
	if(m_hasIndexBuffer) {
		vkCmdDrawIndexed(p_commandBuffer, m_indexCount, 1, 0, 0, 0);
	} else {
		vkCmdDraw(p_commandBuffer, m_vertexCount, 1, 0, 0);
	}
}

} // FFL
