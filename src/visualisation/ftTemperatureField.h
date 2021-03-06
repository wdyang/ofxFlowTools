
#pragma once

#include "ofMain.h"
#include "ftTemperatureFieldShader.h"

namespace flowTools {
	
	class ftTemperatureField {
	public:
		
		void	allocate(int _width, int _height){
			width = _width;
			height = _height;
			
			fieldMesh.setMode(OF_PRIMITIVE_POINTS);
			float xStep = 1. / width;
			float yStep = 1. / height;
			for (int x=0; x<width; x++) {
				for (int y=0; y<height; y++) {
					fieldMesh.addVertex(ofVec3f((x + 0.5) * xStep, (y + 0.5) * yStep, 0));
				}
			}
			fieldVbo.setMesh(fieldMesh, GL_DYNAMIC_DRAW, false, false, false);
			
			
			parameters.setName("temperature field");
			parameters.add(vectorSize.set("vector Size", .25, 0, 1));
			parameters.add(maxSize.set("maxSize", 0.9 / height, 0, 1));
			parameters.add(lineSmooth.set("line smooth", false));
			parameters.add(lineWidth.set("lineWidth", 0.333, 0, 10));
		};
		
		void	draw(int _x, int _y, int _width, int _height) {
			
			ofPushMatrix();
			ofPushStyle();
			
			ofEnableAlphaBlending();
			ofDisableAntiAliasing();
			
			if (lineSmooth.get()) {
				glEnable(GL_LINE_SMOOTH);
			}
			
			glLineWidth(_width / width * lineWidth.get());
			
			ofScale(_width, _height);
			temperatureFieldShader.update(fieldVbo, *floatTexture, vectorSize.get(), maxSize.get());
			
			if (lineSmooth.get()) {
				glDisable(GL_LINE_SMOOTH);
			}
			
			ofEnableAntiAliasing();
			ofPopStyle();
			ofPopMatrix();
		}
		
		void	setSource(ofTexture& tex)	{ floatTexture = &tex; }
		void	setVectorSize(float _value)	{ vectorSize.set(_value); }
		void	setMaxSize(float _value)	{ maxSize.set(_value); }
		void	setLineSmooth(bool _value)	{ lineSmooth.set(_value); }
		void	setLineWidth(float _value)	{ lineWidth.set(_value); }
		
		float	getVectorSize()	{ return vectorSize.get(); }
		float	getMaxSize()	{ return maxSize.get(); }
		bool	getLineSmooth() { return lineSmooth.get(); }
		float	getLineWidth()	{ return lineWidth.get(); }
		int		getWidth()		{ return width; }
		int		getHeight()		{ return height; }
		
		
		ofParameterGroup	parameters;
		
	protected:
		int		width;
		int		height;
		
		ofParameter<float>	vectorSize;
		ofParameter<float>	maxSize;
		ofParameter<bool>	lineSmooth;
		ofParameter<float>	lineWidth;
		
		ofMesh		fieldMesh;
		ofTexture*	floatTexture;
		ofVbo		fieldVbo;
		
		ftTemperatureFieldShader temperatureFieldShader;
		
	};
}

