/******* COPYRIGHT ************************************************
*                                                                 *
*                             FlowVR                              *
*                     Application Library                         *
*                                                                 *
*-----------------------------------------------------------------*
 * COPYRIGHT (C) 2003-2011                by                       *
* INRIA                                                           *
* ALL RIGHTS RESERVED.	                                          *
*                                                                 *
* This source is covered by the GNU LGPL, please refer to the     *
* COPYING file for further information.                           *
*                                                                 *
*-----------------------------------------------------------------*
*                                                                 *
*  Original Contributors:                                         *
*    Jeremie Allard,                                              *
*    Thomas Arcila,                                               *
*    Jean-Denis Lesage.                                           *	
*    Clement Menier,                                              *
*    Bruno Raffin                                                 *
*                                                                 *
*******************************************************************
 *                                                                 *
 *  Contact : Jean-Denis.Lesage@imag.fr                            *
 *                                                                 *
 ******************************************************************/

// basic components includes
#include <flowvr/app/components/metamoduleflowvr-run-ssh-singleton.comp.h>
#include <flowvr/render/components/moduleviewer.comp.h>

// primes specific components includes

#ifndef _METAMODULEVIEWERCUBE_H_
#define _METAMODULEVIEWERCUBE_H_

using namespace flowvr::app;
using namespace flowvr::render;

namespace tutorial {

    // The goal of this component is to encapsulate viewer component and add a command line to launch it
    // ModuleViewer is a generic class with the common interface of viewer components. Use it if your viewer has only a scene and a matrix ports.
class MetaModuleViewerCube : public MetaModuleFlowvrRunSSHSingleton<ModuleViewer>
{
public :
 MetaModuleViewerCube(const std::string& id_ ) : MetaModuleFlowvrRunSSHSingleton<ModuleViewer>(id_, CmdLine("viewercube") ) 
  {
    setInfo("Metamodule launching viewer cube  module");
  };

  //Mandatory virtual destructor
  virtual ~MetaModuleViewerCube(){};
	
  // Mandatory create  method
  virtual Component* create() const { return new MetaModuleViewerCube(getId());};
};

};

#endif //_METAMODULEVIEWERCUBE_H_
