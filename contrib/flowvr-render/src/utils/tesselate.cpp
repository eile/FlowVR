/******* COPYRIGHT ************************************************
*                                                                 *
*                         FlowVR Render                           *
*                   Parallel Rendering Modules                    *
*                                                                 *
*-----------------------------------------------------------------*
* COPYRIGHT (C) 2003-2011                by                       *
* INRIA                                                           *
*  ALL RIGHTS RESERVED.                                           *
*                                                                 *
* This source is covered by the GNU GPL, please refer to the      *
* COPYING file for further information.                           *
*                                                                 *
*-----------------------------------------------------------------*
*                                                                 *
*  Original Contributors:                                         *
*    Jeremie Allard,                                              *
*    Clement Menier.                                              *
*                                                                 * 
*******************************************************************
*                                                                 *
* File: ./src/utils/tesselate.cpp                                   *
*                                                                 *
* Contacts:                                                       *
*                                                                 *
******************************************************************/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <flowvr/utils/cmdline.h>
#include <flowvr/render/mesh.h>

using namespace flowvr::render;

int main(int argc, char** argv)
{
  int rec = 1;
  flowvr::utils::CmdLine cmd("Usage: flowvr-tesselate [options] mesh.input mesh.output");
  cmd.opt("rec",'r',"recursion level of tesselation",&rec);
  bool error=false;
  if (!cmd.parse(argc,argv,&error))
    return error?1:0;

  if (cmd.args.size()!=2)
  {
    std::cerr << cmd.help() << std::endl;
    return 1;
  }

  std::string file_in = cmd.args[0];
  std::string file_out = cmd.args[1];

  Mesh obj;

  if (!obj.load(file_in.c_str()))
  {
    std::cerr << "Failed to read "<<file_in<<std::endl;
    return 1;
  }

  std::cout << "Mesh bbox="<<obj.calcBBox()<<std::endl;

  std::cout << "Flipping mesh..."<<std::endl;
  obj.calcFlip();
  obj.calcEdges();
  bool closed = obj.isClosed();
  std::cout << "Mesh is "<<(closed?"":"NOT ")<<"closed."<<std::endl;

  bool groups = obj.getAttrib(Mesh::MESH_POINTS_GROUP);
  if (!groups)
  {
      std::cout << "Creating artificial groups."<<std::endl;
      // create artificial groups
      for(int i=0;i<obj.nbp();i++)
      {
	  obj.PG(i) = i;
	  obj.GP0(i) = i;
      }
      obj.setAttrib(Mesh::MESH_POINTS_GROUP,true);
  }

  std::cout << "Input mesh: "<<obj.nbp()<<" points, "<<obj.nbf()<<" faces."<<std::endl;

  for (int r=0;r<rec;r++)
  {
      std::cout << "Tesselation level "<<r+1<<"..."<<std::endl;
      obj.calcEdges();

      std::cout << "Creating new points..."<<std::endl;
      // first create a new point on each edge
      for(int e1 = 0 ; e1 < (int)obj.edges.size(); ++e1)
      {
	  int g1 = obj.getPG(e1);
	  for(std::map< int,Mesh::Edge >::iterator it = obj.edges[e1].begin(), itend = obj.edges[e1].end(); it != itend; ++it)
	  {
	      int g2 = obj.getPG(it->first);

	      int f1p1 = -1, f1p2 = -1;
	      int f2p1 = -1, f2p2 = -1;
	      int i1 = -1;
	      if (it->second.f1 >= 0)
	      {
		  Vec3i fp = obj.getFP(it->second.f1);
		  f1p1 = fp[0], f1p2 = fp[1];
		  if (obj.getPG(f1p1) != g1)
		  {
		      f1p1 = fp[1]; f1p2 = fp[2];
		      if (obj.getPG(f1p1) != g1)
		      {
			  f1p1 = fp[2]; f1p2 = fp[0]; // this is the last possible edge
		      }
		  }
		  if (obj.getPG(f1p1) != g1 || (obj.getPG(f1p2) != g2))
		  {
		      std::cerr << "ERROR: Edge "<<g1<<" - "<<g2<<" not found on face 1 ( "<<it->second.f1<<" = "<<fp<<" = "<<Vec3i(obj.getPG(fp[0]),obj.getPG(fp[1]),obj.getPG(fp[2]))<<" )"<<std::endl;
		      it->second.f1 = -1;
		      continue;
		  }
		  Mesh::Vertex v;
		  v = obj.getP(f1p1);
		  v += obj.getP(f1p2);
		  v.mean(2);
		  i1 = obj.addP(v);
		  // replace the face index with the new point index
		  it->second.f1 = i1;
	      }
	      
	      if (it->second.f2 >= 0)
	      {
		  Vec3i fp = obj.getFP(it->second.f2);
		  f2p1 = fp[0], f2p2 = fp[1];
		  if (obj.getPG(f2p1) != g2)
		  {
		      f2p1 = fp[1]; f2p2 = fp[2];
		      if (obj.getPG(f2p1) != g2)
		      {
			  f2p1 = fp[2]; f2p2 = fp[0]; // this is the last possible edge
		      }
		  }
		  if (obj.getPG(f2p1) != g2 || (obj.getPG(f2p2) != g1))
		  {
		      std::cerr << "ERROR: Edge "<<g1<<" - "<<g2<<" not found on face 2 ( "<<it->second.f2<<" = "<<fp<<" = "<<Vec3i(obj.getPG(fp[0]),obj.getPG(fp[1]),obj.getPG(fp[2]))<<" )"<<std::endl;
		      it->second.f2 = -1;
		      continue;
		  }
		  if (f1p1 == f2p2 && f1p2 == f2p1)
		  { // same point as other face
		      it->second.f2 = i1;
		  }
		  else
		  {
		      Mesh::Vertex v;
		      v = obj.getP(f2p1);
		      v += obj.getP(f2p2);
		      v.mean(2);
		      int i2;
		      if (i1 == -1) // no other edge, create a new group
			  i2 = obj.addP(v);
		      else
		      {
			  // see of all points are on the same subgroup
			  int gf1p1 = g1;
			  while (obj.getGP0(gf1p1+1) <= -f1p1) ++gf1p1;
			  int gf1p2 = g2;
			  while (obj.getGP0(gf1p2+1) <= -f1p2) ++gf1p2;
			  int gf2p1 = g2;
			  while (obj.getGP0(gf2p1+1) <= -f2p1) ++gf2p1;
			  int gf2p2 = g1;
			  while (obj.getGP0(gf2p2+1) <= -f2p2) ++gf2p2;
			  int g = obj.getPG(i1);
			  i2 = obj.addP(v,g);
			  if (gf1p1 != gf2p2 || gf1p2 != gf2p1)
			  { // create a subgroup
			      obj.GP0(obj.nbg()) = -i2;
			  }
		      }
		      // replace the face index with the new point index
		      it->second.f2 = i2;
		  }
	      }
	  }
      }

      // then create new faces
      std::cout << "Creating new faces..."<<std::endl;
      std::vector<Vec3i> faces_p;
      int nbf0 = obj.nbf();
      faces_p.reserve(nbf0*4);
      for(int i=0; i<nbf0; ++i)
      {
	  Vec3i points = obj.getFP(i);
	  Vec3i edges;
	  edges[0] = obj.getEdgeFace(points[0],points[1]);
	  edges[1] = obj.getEdgeFace(points[1],points[2]);
	  edges[2] = obj.getEdgeFace(points[2],points[0]);
	  if ((unsigned)edges[0] >= obj.nbp() || (unsigned)edges[1] >= obj.nbp() || (unsigned)edges[2] >= obj.nbp())
	  {
	      std::cerr << "ERROR: invalid edge points " << edges << " in face " <<i<<" = "<<points<<std::endl;
	      continue;
	  }
	  faces_p.push_back(Vec3i(edges[2],points[0],edges[0]));
	  faces_p.push_back(Vec3i(edges[0],points[1],edges[1]));
	  faces_p.push_back(Vec3i(edges[2],edges[0],edges[1]));
	  faces_p.push_back(Vec3i(edges[1],points[2],edges[2]));
      }
      obj.faces_p = faces_p;

      // finally we update the material groups
      std::cout << "Updating materials..."<<std::endl;
      for(unsigned int i=0; i<obj.mat_groups.size(); ++i)
      {
	  obj.mat_groups[i].f0 *= 4;
	  obj.mat_groups[i].nbf *= 4;
      }
      // and we recompute the edges
      if (r < rec-1)
      {
	  std::cout << "Updating edges..."<<std::endl;
	  obj.calcEdges();
	  bool closed = obj.isClosed();
	  std::cout << "Mesh is "<<(closed?"":"NOT ")<<"closed."<<std::endl;
      }
      std::cout << "Tesselation level "<<r<<" DONE: "<<obj.nbp()<<" points, "<<obj.nbf()<<" faces."<<std::endl;
  }
  if (!groups)
  {
      // remove artificial groups
      obj.setAttrib(Mesh::MESH_POINTS_GROUP,false);
  }
  obj.calcNormals();
  std::cout << "Saving result..."<<std::endl;
  obj.save(file_out.c_str());
  return 0;
}
