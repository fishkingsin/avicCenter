#!/bin/bash

# Dependencies for avicCenter

#make sure you are in the avicCenter project when you run this script

cd ../../addons

#passing no arguments means read only
if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
  echo "Cloning read-only"
else
    PREFIX="git clone git@github.com:"
	echo "Cloning writable"
fi


${PREFIX}fishkingsin/ofxRenderManager
${PREFIX}armadillu/ofxAnimatable
${PREFIX}rezaali/ofxUI
${PREFIX}astellato/ofxSyphon
${PREFIX}fishkingsin/ofxDraggable
${PREFIX}memo/ofxMSAInteractiveObject
