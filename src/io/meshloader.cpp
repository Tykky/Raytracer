#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "meshloader.h"
#include "hittables/Triangle.h"
#include <string>
#include <vector>

Mesh loadObj(const std::string &file) {
    tinyobj::ObjReaderConfig reader_config;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(file, reader_config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    for (const auto & shape : shapes) {

        auto tri_count = static_cast<unsigned int>(shape.mesh.num_face_vertices.size());
        std::vector<Triangle> triangles(tri_count);

        // Loop over faces(triangles)
        size_t index_offset = 0;
        for (size_t f = 0; f < tri_count; f++) {
            int fv = shape.mesh.num_face_vertices[f];

            if (fv != 3) {
                std::cerr << "loadObj: Incorrect number of vertices (not triangle)" << std::endl;
                exit(1);
            }

            Triangle triangle = {};
            Vector3D normal = {};

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
                normal += Vector3D(static_cast<float>(nx), static_cast<float>(ny), static_cast<float>(nz));
                triangle.setVertex(Vector3D(static_cast<float>(vx), static_cast<float>(vy), static_cast<float>(vz)), v);
            }
            index_offset += fv;
            normal /= static_cast<float>(fv); // average normals
            triangles[f] = triangle;
        }
    }

    return Mesh();
}