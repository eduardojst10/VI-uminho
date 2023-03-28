#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
#include "../Primitive/primitive.hpp"
#include "../Primitive/Geometry/mesh.hpp"
#include "../Primitive/BRDF/Phong.hpp"
#include "scene.hpp"


#include <iostream>
#include <set>
#include <vector>
//#include "spdlog/spdlog.h"

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
    tinyobj::ObjReader myObjReader;
    bool success = false;
    
    // Loader triangulating the faces
    if (!myObjReader.ParseFromFile(fname)) {
        if (!myObjReader.Error().empty()) {std::cerr << "TinyObjReader: " << myObjReader.Error();}  
        return false;
    }
    // convert loader's representation to my representation
    const std::vector<shape_t> shapes = myObjReader.GetShapes();
    const std::vector<material_t> materials = myObjReader.GetMaterials();
    attrib_t attrib = myObjReader.GetAttrib();

    // Load Materials
    for(const auto& mat: materials){
        
        Phong *p = new Phong();
        p->Ka = RGB(mat.ambient[0],mat.ambient[1],mat.ambient[2]);
        p->Kd = RGB(mat.diffuse[0],mat.diffuse[1],mat.diffuse[2]);
        p->Ks = RGB(mat.specular[0],mat.specular[1],mat.specular[2]);
        p->Kt = RGB(mat.transmittance[0],mat.transmittance[1],mat.transmittance[2]);
        p->Ns = mat.shininess;
        std::cout << p->Ka.R << p->Ka.G << p->Ka.B << std::endl;
        this->BRDFs.push_back(p);
        this->numBRDFs+=1;
    }
    Mesh *mesh = new Mesh(); 
    for (size_t i = 0; i < attrib.vertices.size(); i += 3) {
        real_t vx = attrib.vertices[i];
        real_t vy = attrib.vertices[i + 1];
        real_t vz = attrib.vertices[i + 2];
        mesh->vertices.push_back(Point(vx, vy, vz));
    }
    //Load Primitives
    for(const auto& shape: shapes){ //Shapes (individual objects or meshes)
        int index_offset = 0;
        std::vector<Face> newFaces;
        Primitive* primitive = new Primitive();
        for(size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++){
            size_t fv = size_t(shape.mesh.num_face_vertices[f]);
            // Only supports triangles
            if (fv != 3) {
                std::cerr << "-- Error: Only triangles are supported --" << std::endl;
                return false;
            }

            // Load vertices of the current face: indices to our internal vector of vertices (in Mesh)
            //BB& bb = face.bb;
            //bb.min = bb.max = mesh->vertices[face.vert_ndx[0]]; //erro está aqui
            Face face; 
            for (size_t i = 0; i < fv; i++) {
                index_t idx = shape.mesh.indices[index_offset + i];
                face.vert_ndx[i] = idx.vertex_index;  //loading of vertex values
                face.vert_normals_ndx[i] = idx.normal_index;
                //std::cout << "In vertex: " << idx.vertex_index << "\n"; 
                //real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                //real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                //real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];
                //mesh->vertices.push_back(Point(vx,vy,vz));
                /*
                    TODO:
                     Perguntar ao stor a variável bool hasShadingNormals, como obter?
                     Se temos check whether the normal_index field is valid or not for each vertex, and if it is, 
                     we store the index of the shading normal in the face.vert_normals_ndx array and set the hasShadingNormals 
                     flag to true
                */
                if(idx.normal_index >=0){
                    face.vert_normals_ndx[i] = idx.normal_index;
                    face.hasShadingNormals = true;
                }else face.hasShadingNormals = false;

                /*
                   Bounding box (BB) to the position of the first vertex and then updates it for each subsequent vertex by 
                   computing the minimum and maximum coordinates along each axis. After the loop, the bounding box will 
                   contain the minimum and maximum coordinates of all vertices of the face. 
                */ 
                //const Point& p = mesh->vertices[face.vert_ndx[i]];

                /*
                    bb.min.X = std::min(bb.min.X , p.X );
                    bb.min.Y = std::min(bb.min.Y, p.Y);
                    bb.min.Z = std::min(bb.min.Z, p.Z);
                    bb.max.X = std::max(bb.max.X, p.X);
                    bb.max.Y = std::max(bb.max.Y, p.Y);
                    bb.max.Z = std::max(bb.max.Z, p.Z);
                */
            }

            // Calculate geometric normal
            Vector v0(attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 0].vertex_index) + 0],
                   attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 0].vertex_index) + 1],
                   attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 0].vertex_index) + 2]);
            Vector v1(attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 1].vertex_index) + 0],
                   attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 1].vertex_index) + 1],
                   attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 1].vertex_index) + 2]);
            Vector v2(attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 2].vertex_index) + 0],
                   attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 2].vertex_index) + 1],
                   attrib.vertices[3*size_t(shape.mesh.indices[index_offset + 2].vertex_index) + 2]);
            Vector geonormal = (v1 - v0).cross(v2-v0);
            geonormal.normalize();
            face.geoNormal = geonormal;
            mesh->faces.push_back(face);
            //Próximo vertice, every 3 in array
            index_offset +=fv;
            primitive->material_ndx = shape.mesh.material_ids[f];


        }
       //Conversions between related types in a safe and predictable way static_cast<new_type>(expression)
        mesh->numFaces = static_cast<int>(mesh->faces.size());
        mesh->numVertices = static_cast<int>(attrib.vertices.size() / 3);
        
        // Load unique normals
        for (size_t i = 0; i < attrib.normals.size(); i += 3) {
            Vector normal(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]);
            mesh->normals.push_back(normal);
        }

        mesh->numNormals = static_cast<int>(mesh->normals.size());
        /*
            The reason we use a pointer to a Geometry object (Geometry*) instead of a Geometry object directly 
            is that Geometry is an abstract base class, meaning it has pure virtual functions that must 
            be implemented by any derived class.
        
            Dps de dar load aos vertices, normals e faces vamos criar a nova Primitiva             
          
        */
        Geometry* geometry = mesh;
        primitive->g = geometry;
        this->prims.push_back(primitive);
        this->numPrimitives++;
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
        if ((*prim_itr)->g->intersect(r, &curr_isect)) {
            if (!intersection) { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = BRDFs[(*prim_itr)->material_ndx];
            }
            else if (curr_isect.depth < isect->depth) {
                *isect = curr_isect;
            }
        }
    }
    return intersection;
}

