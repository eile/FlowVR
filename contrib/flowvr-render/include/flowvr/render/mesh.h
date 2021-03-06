/******* COPYRIGHT ************************************************
*                                                                 *
*                         FlowVR Render                           *
*                   Parallel Rendering Library                    *
*                                                                 *
*-----------------------------------------------------------------*
* COPYRIGHT (C) 2003-2011                by                       *
* INRIA                                                           *
*  ALL RIGHTS RESERVED.                                           *
*                                                                 *
* This source is covered by the GNU LGPL, please refer to the     *
* COPYING-LIB file for further information.                       *
*                                                                 *
*-----------------------------------------------------------------*
*                                                                 *
*  Original Contributors:                                         *
*    Jeremie Allard,                                              *
*    Clement Menier.                                              *
*                                                                 * 
*******************************************************************
*                                                                 *
* File: ./include/flowvr/render/mesh.h                            *
*                                                                 *
* Contacts:                                                       *
*                                                                 *
******************************************************************/
#ifndef FLOWVR_RENDER_MESH_H
#define FLOWVR_RENDER_MESH_H

#include <ftl/type.h>
#include "bbox.h"
#include "chunkrenderwriter.h"

#include <vector>
#include <map>
#include <stdio.h>

namespace flowvr
{

namespace render
{

/// Mesh container helper class
class Mesh
{
public:
  enum { MESH_MAGIC = 0x003E583E, MESH_MAGIC_SWAP = 0x3E583E00 };

  enum Attrib {
    MESH_POINTS_POSITION=1<<0,
    MESH_POINTS_TEXCOORD=1<<1,
    MESH_POINTS_NORMAL  =1<<2,
    MESH_POINTS_COLOR   =1<<3,
    MESH_POINTS_VALUE   =1<<4,
    MESH_POINTS_GROUP   =1<<5,
    MESH_FACES          =1<<6,
    MESH_DISTMAP        =1<<7,
    MESH_VOXEL          =1<<8,
    MESH_MATERIALS      =1<<9,
  };

  int attrib;

  bool getAttrib(Attrib a) const { return attrib&(a); }
  void setAttrib(Attrib a, bool b) { if(b) attrib|=(a); else attrib&=~(a); }

  struct Vertex //__attribute__((packed))
  {
    Vec3f p;
    Vec2f t;
    Vec3f n;
    Vec4b c;
    float v;
    void operator+=(const Vertex& b)
    {
      p+=b.p;
      t+=b.t;
      n+=b.n;
      c+=b.c;
      v+=b.v;
    }
    void mean(int nb)
    {
      p/=nb;
      t/=nb;
      n.normalize();
      c/=nb;
      v/=nb;
    }
  };

  std::vector<Vec3f> points_p;

  Vec3f getPP(int i) const { if ((unsigned)i<points_p.size()) return points_p[i]; else return Vec3f(); }
  Vec3f& PP(int i) { if ((unsigned)i>=points_p.size()) points_p.resize(i+1); return points_p[i]; }

  Vec2f getPT(int i) const { if ((unsigned)i<points_t.size()) return points_t[i]; else return Vec2f(); }
  Vec2f& PT(int i) { if ((unsigned)i>=points_t.size()) points_t.resize(i+1); return points_t[i]; }

  Vec3f getPN(int i) const { if ((unsigned)i<points_n.size()) return points_n[i]; else return Vec3f(); }
  Vec3f& PN(int i) { if ((unsigned)i>=points_n.size()) points_n.resize(i+1); return points_n[i]; }

  Vec4f getPC(int i) const { if ((unsigned)i<points_c.size()) return points_c[i]; else return Vec4f(); }
  Vec4f& PC(int i) { if ((unsigned)i>=points_c.size()) points_c.resize(i+1); return points_c[i]; }

  float getPV(int i) const { if ((unsigned)i<points_v.size()) return points_v[i]; else return 0; }
  float& PV(int i) { if ((unsigned)i>=points_v.size()) points_v.resize(i+1); return points_v[i]; }

  Vertex getP(int i) const {
    Vertex v;
    v.p = getPP(i);
    v.t = getPT(i);
    v.n = getPN(i);
    v.c = getPC(i);
    v.v = getPV(i);
    return v;
  }

  int addP(const Vertex& v, int g=-1) {
    int i = nbp();
    PP(i) = v.p;
    PN(i) = v.n;
    if (getAttrib(MESH_POINTS_TEXCOORD)) PT(i) = v.t;
    if (getAttrib(MESH_POINTS_COLOR   )) PC(i) = v.c;
    if (getAttrib(MESH_POINTS_VALUE   )) PV(i) = v.v;
    if (getAttrib(MESH_POINTS_GROUP   ))
    {
      if (g==-1) g=nbg();
      PG(i) = g;
      if (g>=nbg())
	GP0(g) = i;
    }
    return i;
  }

  int getPG(int i) const { if ((unsigned)i<points_g.size()) return points_g[i]; else return 0; }
  int& PG(int i) { if ((unsigned)i>=points_g.size()) points_g.resize(i+1); return points_g[i]; }

  int getGP0(int i) const { if ((unsigned)i<groups_p0.size()) return groups_p0[i]; else return -1; }
  int& GP0(int i) { if ((unsigned)i>=groups_p0.size()) groups_p0.resize(i+1); return groups_p0[i]; }

  Vec3i getFP(int i) const { if ((unsigned)i<faces_p.size()) return faces_p[i]; else return Vec3i(); }
  Vec3i& FP(int i) { if ((unsigned)i>=faces_p.size()) faces_p.resize(i+1); return faces_p[i]; }

  Vec3f getFN(int i) const { if ((unsigned)i<faces_n.size()) return faces_n[i]; else return Vec3f(); }
  Vec3f& FN(int i) { if ((unsigned)i>=faces_n.size()) faces_n.resize(i+1); return faces_n[i]; }

  Vec3f getFU(int i) const { if ((unsigned)i<faces_u.size()) return faces_u[i]; else return Vec3f(); }
  Vec3f& FU(int i) { if ((unsigned)i>=faces_u.size()) faces_u.resize(i+1); return faces_u[i]; }

  Vec3f getFV(int i) const { if ((unsigned)i<faces_v.size()) return faces_v[i]; else return Vec3f(); }
  Vec3f& FV(int i) { if ((unsigned)i>=faces_v.size()) faces_v.resize(i+1); return faces_v[i]; }

  int addF(Vec3i f)
  {
    int i = nbf();
    FP(i) = f;
    if (!edges.empty())
    {
      addEdgeFace(f[0],f[1],i);
      addEdgeFace(f[1],f[2],i);
      addEdgeFace(f[2],f[0],i);
    }
    return i;
  }

  std::vector<Vec2f> points_t;
  std::vector<Vec3f> points_n;
  std::vector<Vec4f> points_c;
  std::vector<float> points_v;

  std::vector<int> points_g;
  std::vector<int> groups_p0;

/*
  struct Face //__attribute__((packed))
  {
    int p[3];
    Vec3f n;
    Vec3f u; // Vector to compute u barycentric coordinate
    Vec3f v; // Vector to compute v barycentric coordinate
  };
*/
  std::vector<Vec3i> faces_p;
  std::vector<Vec3f> faces_n;
  std::vector<Vec3f> faces_u;
  std::vector<Vec3f> faces_v;

  struct Edge
  {
    int f1,f2;
    Edge() : f1(-1), f2(-1) {}
  };

  std::vector< std::map< int,Edge > > edges;

  struct DistMap
  {
    int nx;
    int ny;
    int nz;
    float maxDist;
    Mat4x4f mat;
    float data[0];
    int nval() const { return nx*ny*nz; }
    int size() const { return sizeof(DistMap)+nval()*sizeof(float); }
  };

  DistMap* distmap;

  struct Voxel
  {
    int nx;
    int ny;
    int nz;
    Mat4x4f mat;
    unsigned char data[0]; ///< bit-field containing the voxels, byte-aligned at each line
    int size() const { return sizeof(Voxel)+((nx+7)/8)*ny*nz; }
  };

  Voxel* voxel;

  struct Material
  {
    std::string mtllib; ///< Name of file containing material
    std::string matname; ///< Name of material
    Vec3f ambient, diffuse, specular, emmisive;
    float alpha;
    float shininess;
    std::string map_ambient, map_diffuse, map_specular, map_emmisive, map_alpha;
    std::string map_bump;

    Material()
    : alpha(1), shininess(0)
    {
    }
  };

  struct MaterialGroup
  {
    std::string gname; ///< Name of group of faces using this material
    std::string matname; ///< Name of material
    Material* mat; ///< Material itself
    int f0, nbf; ///< List of faces in this material group
    MaterialGroup()
    : mat(NULL), f0(0), nbf(0)
    {
    }
  };

  std::map<std::string, Material*> mat_map;

  Material* getM(const std::string& name)
  {
    std::map<std::string, Material*>::iterator it = mat_map.find(name);
    if (it == mat_map.end()) return NULL;
    else return it->second;
  }

  std::vector<MaterialGroup> mat_groups;

  BBox bb;

  int nbp() const { return points_p.size(); }
  int nbf() const { return faces_p.size(); }
  int nbg() const { return groups_p0.size(); }

  int nbmatg() const { return mat_groups.size(); }

  Mesh();

  ~Mesh();

  void operator=(const Mesh& mesh);

  void clear();
  void init(int nbp, int nbf, int attribs=MESH_POINTS_POSITION|MESH_FACES);

  bool load(const char* filename);
  bool loadMesh(const char* filename);
  bool loadObj(const char* filename, const char* filter=NULL);
  bool loadOff(const char* filename);
  bool save(const char* filename) const;
  bool saveMesh(const char* filename) const;
  bool saveObj(const char* filename) const;
  bool saveOff(const char* filename) const;
  bool saveStl(const char* filename) const;
  bool saveObj(FILE* f, int &v0, int &vn0, int &vt0) const;
  bool saveObj(FILE* f, int &v0) const
  {
    int vn0=1;
    int vt0=1;
    return saveObj(f,v0,vn0,vt0);
  }
  bool saveObj(FILE* f) const
  {
    int v0=1;
    return saveObj(f,v0);
  }

  /// Output the mesh with FlowVR-Render
  void writeMesh(ChunkRenderWriter* scene, ID idIB, ID idVB, int gen=-1) const;
  void writeParams(ChunkRenderWriter* scene, ID idP, ID idIB, ID idVB) const;
  /**
	* \brief Output the mesh with two FlowVR messages (Points and Facets)
	* \param flowvr pointer to the FlowVR modeule descriptor
	* \param points pointer to the message with points (3 unsigned short)
	* \param facets pointer to the message with facets (3 float)
	*/
  void writeMeshFacetsPointsMessages(ModuleAPI* flowvr, MessageWrite* points, MessageWrite* facets) const;
 /**
	* \brief Output the color of the mesh with a FlowVR message 
	* \param flowvr pointer to the FlowVR modeule descriptor
	* \param colors pointer to the message with colors (4 bytes)
	*/

  void writeMeshColorPointsMessage(ModuleAPI* flowvr, MessageWrite* colors) const;

  /**
	* \brief Read the mesh from two FlowVR messages (Points and Facets)
	* \param points reference to the message with points (3 unsigned short)
	* \param facets reference to the message with facets (3 float)
	*/
  void readMeshFacetsPointsMessages(const Message& points, const Message& facets);

  /**
	* \brief Read the mesh from FlowVR message (color)
	* \param points reference to the message with colors (4 bytes)
	*/
  void readMeshColorPointsMessage(const Message& colors);

  /// Compute face normals and points normals (if not already specified or if force is true)
  void calcNormals(bool force=false);

  /// Compute mesh bounding box
  BBox calcBBox();

  /// Compute bounding box of a submesh
  BBox calcBBox(int f0, int nbf);

  /// Compute bounding box of a material group
  BBox calcBBox(int matg)
  {
      return calcBBox(mat_groups[matg].f0, mat_groups[matg].nbf);
  }

  /// Compute edges data
  void calcEdges();

  /// Flip all faces
  void flipAll();

  /// Flip faces not correctly oriented
  void calcFlip();

  /// Check if the mesh is closed
  bool isClosed();

  /// Close the mesh. Note: the mesh must be correctly flipped
  void close();

  /// Brute-force distance computation. Note: the mesh must be closed
  float calcDist(Vec3f pos) const;

  template<class Real>
  void calcExtDistMap(Mat4x4f mat, Real* dest, int nx, int ny, int nz, float maxDist, float fact = 1);

  void calcDistMap(Mat4x4f mat, int nx, int ny, int nz, float maxDist);

  void calcDistMap(int nx, int ny, int nz, float maxDist);

  void calcDistMap(int n, float maxDist);

  void translate(Vec3f d);

  void dilate(float dist);

  /// Optimize mesh by merging identical groups and reordering faces
  void optimize();

  /// Return true if some materials requires tangent vectors
  bool needTangents() const;

  /// Compute the tangent and co-tangent at each point
  void calcTangents(std::vector<Vec3f>& tangent1, std::vector<Vec3f>& tangent2) const;

  /// Compute the tangent at each point, plus a fourth coordinate indicating the direction of the co-tangent.
  /// See http://www.terathon.com/code/tangent.php
  void calcTangents(std::vector<Vec4f>& tangent) const;

  int getEdgeFace(int p0, int p1);



protected:

  void clearEdges();
  
  bool addEdgeFace(int p0, int p1, int f);

  void closeLoop(const std::vector<int>& loop);

};

} // namespace render

} // namespace flowvr

#endif
