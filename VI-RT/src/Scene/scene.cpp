#include "scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
#include "../Primitive/primitive.hpp"
#include "../Primitive/Geometry/mesh.hpp"
#include "../Primitive/BRDF/Phong.hpp"

#include <iostream>
#include <set>
#include <vector>

using namespace tinyobj;

static void PrintInfo (const ObjReader myObj) {
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
              << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
    
    // Iterate shapes
    auto it_shape = shapes.begin();
    for ( ; it_shape != shapes.end() ; it_shape++) {
        // assume each face has 3 vertices
        std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for ( ; it_vertex != it_shape->mesh.indices.end() ; ) {
            // process 3 vertices
            for (int v=0 ; v<3 ; v++) {
                std::cout << it_vertex->vertex_index;
                it_vertex++;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        
        printf("There are %zu material indexes\n", it_shape->mesh.material_ids.size());
    }
    
}

/*

COMEÇAR AQUI
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 
int numPrimitives, numLights, numBRDFs;
    std::vector <Primitive> prims;
    std::vector <Light> lights;
    std::vector <BRDF> BRDFs; 
 
 
 
*/

 bool Scene::Load (const std::string &fname) {
    ObjReaderConfig reader_config;
    ObjReader myObjReader;
    bool success = false;


    // this loader triangulates the faces
    if (!myObjReader.ParseFromFile(fname,reader_config)) {
        if (!myObjReader.Error().empty()){
            std::cerr << "TinyObjReader: " << myObjReader.Error();
        }
        return false;
    }


    if(!myObjReader.Warning().empty()){
        std::cout << "TinyObjReader Warning: " << myObjReader.Warning();
    }
    // convert loader's representation to my representation
    const std::vector<shape_t> shapes = myObjReader.GetShapes();
    const std::vector<material_t> materials = myObjReader.GetMaterials();
    attrib_t attrib = myObjReader.GetAttrib();
    /*
    auto keyword in C++ automatically detects and assigns a data type to the variable with which it is used. 
    The compiler analyses the variable's data type by looking at its initialization.
    Podemos usar size_t ou auto
     
    Shapes (individual objects or meshes)
    */
    
    for(const auto& shape: shapes){
        int index_offset = 0;
        Mesh *mesh; 
        
        
        std::vector<Face> newFaces;
        for(size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f){
            int fv = shape.mesh.num_face_vertices[f];
            // Only supports triangles
            if (fv != 3) {
                std::cerr << "Error: Only triangles are supported." << std::endl;
                return false;
            }

            // Load vertices of the current face: indices to our internal vector of vertices (in Mesh)
            Face face;
            for (int i = 0; i < fv; i++) {
                index_t idx = shape.mesh.indices[index_offset + i];
                face.vert_ndx[i] = idx.vertex_index;
                if(idx.normal_index >=0){
                    face.vert_normals_ndx[i] = idx.normal_index;
                    face.hasShadingNormals = true;
                }else face.hasShadingNormals = false;
                
                // optional: per-vertex material IDs
                // size_t material_id = shapes[s].mesh.material_ids[f];
            }

            // Calculate geometric normal
            Vector v0(mesh->vertices[face.vert_ndx[0]].X,mesh->vertices[face.vert_ndx[0]].Y, mesh->vertices[face.vert_ndx[0]].Z);
            Vector v1(mesh->vertices[face.vert_ndx[1]].X,mesh->vertices[face.vert_ndx[1]].Y, mesh->vertices[face.vert_ndx[1]].Z);
            Vector v2(mesh->vertices[face.vert_ndx[2]].X,mesh->vertices[face.vert_ndx[2]].Y, mesh->vertices[face.vert_ndx[2]].Z);
            Vector geonormal = (v1 - v0).cross(v2-v0); //normalize?
            face.geoNormal = geonormal;

            mesh->faces.push_back(face);
            //Próximo vertice, andamos de 3 em 3 no array
            index_offset +=fv;

            /*
                The reason we use a pointer to a Geometry object (Geometry*) instead of a Geometry object directly 
                is that Geometry is an abstract base class, meaning it has pure virtual functions that must 
                be implemented by any derived class.
        
                Dps de dar load aos vertices, normals e faces vamos criar uma nova primitive
            
               
            
            Geometry* geometry = new Mesh(mesh);
            Primitive primitive;
            //Vou ter problemas ocm pointers? usar "->"
            primitive.g = geometry;
            primitive.material_ndx = 0;
            */
        }

        //mesh->numFaces = shape.mesh.num_face_vertices.size();??
        //conversions between related types in a safe and predictable way
        //static_cast<new_type>(expression)
        mesh->numFaces = static_cast<int>(mesh->faces.size());
        mesh->numVertices = static_cast<int>(attrib.vertices.size() / 3);

      // Load vertices
        for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
            Point vertex(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
            mesh->vertices.push_back(vertex);
        }

        // Load normals
        for (size_t i = 0; i < attrib.normals.size(); i += 3) {
            Vector normal(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]);
            mesh->normals.push_back(normal);
        }

        mesh->numNormals = static_cast<int>(mesh->normals.size());
    
    }
    
    //PrintInfo(myObjReader);
        
    return true;
}

bool SetLights (void) { return true; }

bool Scene::trace (Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;    
    
    if (numPrimitives==0) return false;
    
    // iterate over all primitives
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        if (prim_itr->g->intersect(r, &curr_isect)) {
            if (!intersection) { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = &BRDFs[prim_itr->material_ndx];
            }
            else if (curr_isect.depth < isect->depth) {
                *isect = curr_isect;
            }
        }
    }
    return intersection;
}

