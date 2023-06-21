#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
#include "../Primitive/primitive.hpp"
#include "../Primitive/Geometry/mesh.hpp"
#include "../Primitive/BRDF/Phong.hpp"
#include "scene.hpp"


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


bool Scene::Load (const std::string &fname) {
    tinyobj::ObjReader myObjReader;
    // Loader triangulating the faces
    if (!myObjReader.ParseFromFile(fname)) {
        if (!myObjReader.Error().empty()) {std::cerr << "TinyObjReader: " << myObjReader.Error();}
        return false;
    }
    //PrintInfo(myObjReader);
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
        //std::cout << p->Ka.R << p->Ka.G << p->Ka.B << std::endl;
        this->BRDFs.push_back(p);
        this->numBRDFs+=1;
    }

    //Load Primitives
    for(const auto& shape: shapes){ //Shapes (individual objects or meshes)
        Mesh *mesh = new Mesh();
        Primitive* primitive = new Primitive();
        primitive->name = shape.name;
        //std::cout << "PRIMITIVE name: " << primitive->name << "\n";
        // Load unique vertices
        for (size_t i = 0; i < attrib.vertices.size(); i += 3) {

            tinyobj::real_t vx = attrib.vertices[i];
            tinyobj::real_t vy = attrib.vertices[i + 1];
            tinyobj::real_t vz = attrib.vertices[i + 2];
            mesh->vertices.push_back(Point(vx, vy, vz));
        }
        mesh->numVertices = static_cast<int>(mesh->vertices.size());
        int index_offset = 0;

        // Percorrer as faces de cada primitiva - cada face é um trinagulo
        for(size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++){
            size_t fv = size_t(shape.mesh.num_face_vertices[f]);
            // Only supports triangles
            if (fv != 3) {
                std::cerr << "-- Error: Only triangles are supported --" << std::endl;
                return false;
            }
            Face face;
            BB boundingbox;
            for (size_t i = 0; i < fv; i++) {
                index_t idx = shape.mesh.indices[index_offset + i];
                face.vert_ndx[i] = idx.vertex_index;  //loading of vertex values
                face.vert_normals_ndx[i] = idx.normal_index;

                /*
                    TODO:
                     variável bool hasShadingNormals:
                     Se temos check whether the normal_index field is valid or not for each vertex, and if it is,
                     we store the index of the shading normal in the face.vert_normals_ndx array and set the hasShadingNormals
                     flag to true
                */
                if(idx.normal_index >=0){
                    face.vert_normals_ndx[i] = idx.normal_index;
                    face.hasShadingNormals = true;
                }else face.hasShadingNormals = false;

                Point p = mesh->vertices[face.vert_ndx[i]];
                boundingbox.min.X = std::min(boundingbox.min.X , p.X );
                boundingbox.min.Y = std::min(boundingbox.min.Y, p.Y);
                boundingbox.min.Z = std::min(boundingbox.min.Z, p.Z);
                boundingbox.max.X = std::max(boundingbox.max.X, p.X);
                boundingbox.max.Y = std::max(boundingbox.max.Y, p.Y);
                boundingbox.max.Z = std::max(boundingbox.max.Z, p.Z);

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
            face.bb = boundingbox;
            mesh->faces.push_back(face);
            mesh->primitive = primitive->name;
            //Próximo vertice, every 3 in array
            index_offset +=fv;
            primitive->material_ndx = shape.mesh.material_ids[f];
            /*
            if(primitive->name == "short_block" || primitive->name == "tall_block"){
                //std::cout << "Material index for face " << f << ": " << shape.mesh.material_ids[f] << std::endl;

            }
            */
            //Conversions between related types in a safe and predictable way static_cast<new_type>(expression)
            mesh->numFaces = static_cast<int>(shape.mesh.num_face_vertices.size());
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
        }
        Geometry* geometry = mesh;
        primitive->g = geometry;
        this->prims.push_back(primitive);
        this->numPrimitives++;

    }
    return true;
}

bool Scene::trace(Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;
    if (numPrimitives==0) return false;

    // iterate over all primitives
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        if ((*prim_itr)->g->intersect(r, &curr_isect)) {
            if (!intersection || curr_isect.depth < isect->depth) { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = this->BRDFs[(*prim_itr)->material_ndx];
                isect->isLight = false;
            }

        }
    }
    for(auto l = lights.begin(); l != lights.end(); l++){
        if ((*l)->type == AREA_LIGHT) {
            AreaLight *al = (AreaLight *)*l;//static_cast<AreaLight*> (*l);
            if (al->gem->intersect(r, &curr_isect)) {
                if (!intersection || curr_isect.depth < isect->depth) {
                    intersection = true;
                    *isect = curr_isect;
                    isect->isLight = true;
                    isect->Le = al->L();
                    isect->f = nullptr;
                }
            }
        }
    }

    return intersection;
}

// checks whether a point on a light source (distance maxL) is visible
// Similar to Scene::trace() but finishes immediately once the 1st intersection is found
bool Scene::visibility (Ray s, const float maxL) {
    bool visible = true;
    Intersection curr_isect;

    if (this->numPrimitives==0) return true;

    // iterate over all primitives while visible
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() && visible ; prim_itr++) {
        if ((*prim_itr)->g->intersect(s, &curr_isect)) {
            if (curr_isect.depth < maxL) {
                visible = false;
            }
        }
    }
    return visible;
}