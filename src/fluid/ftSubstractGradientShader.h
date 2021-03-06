
#pragma once

#include "ofMain.h"
#include "ftShader.h"

namespace flowTools {
	
	class ftSubstractGradient : public ftShader {
	public:
		ftSubstractGradient() {
			
			fragmentShader = STRINGIFY(uniform sampler2DRect Velocity;
									   uniform sampler2DRect Pressure;
									   uniform sampler2DRect Obstacle;
									   uniform float HalfInverseCellSize;
									   
									   void fTexNeighbors(sampler2DRect tex, vec2 st,
														  out float left, out float right, out float bottom, out float top) {
										   left   = texture2DRect(tex, st - vec2(1, 0)).x;
										   right  = texture2DRect(tex, st + vec2(1, 0)).x;
										   bottom = texture2DRect(tex, st - vec2(0, 1)).x;
										   top    = texture2DRect(tex, st + vec2(0, 1)).x;
									   }
									   
									   void main(){
										   
										   vec2 st = gl_TexCoord[0].st;										   
										   
										   float pL; float pR; float pB; float pT;
										   fTexNeighbors (Pressure, st, pL, pR, pB, pT);
										   float pC = texture2DRect(Pressure, st).x;
										   
										   float oL; float oR; float oB; float oT;
										   fTexNeighbors (Obstacle, st, oL, oR, oB, oT);
										   
										   vec2 vMask = vec2(1.0,1.0);
										   
										   if (oL > 0.9) { pL = pC; vMask.x = 0.0; }
										   if (oR > 0.9) { pR = pC; vMask.x = 0.0; }
										   if (oB > 0.9) { pB = pC; vMask.y = 0.0; }
										   if (oT > 0.9) { pT = pC; vMask.y = 0.0; }
										   
										   vec2 oldV = texture2DRect(Velocity, st).xy;
										   vec2 grad = vec2(pR - pL, pT - pB) * HalfInverseCellSize;
										   vec2 newV = oldV - grad;
										   
										   gl_FragColor = vec4((vMask * newV), 0.0, 0.0);
									   }
									   );
			
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			shader.linkProgram();
		}
		
		void update(ofFbo& _buffer, ofTexture& _backBufferTexture, ofTexture& pressureTexture, ofTexture& _obstaclesTexture, float _cellSize){
			
			_buffer.begin();
			shader.begin();
			shader.setUniform1f("HalfInverseCellSize", 0.5f / _cellSize);
			
			shader.setUniformTexture("Velocity", _backBufferTexture, 0);
			shader.setUniformTexture("Pressure", pressureTexture, 1);
			shader.setUniformTexture("Obstacle", _obstaclesTexture, 2);
			renderFrame(_buffer.getWidth(), _buffer.getHeight());
			
			shader.end();
			_buffer.end();
		}
	};
}