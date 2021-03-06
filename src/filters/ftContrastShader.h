
#pragma once
#include "ofMain.h"
#include "ftShader.h"

namespace flowTools {

	class ftContrastShader : public ftShader {
	public:
		ftContrastShader(){
			
			fragmentShader = STRINGIFY(uniform sampler2DRect tex0;
									   uniform float contrast;
									   uniform float brightness;
									   
									   void main(){
										   vec4 color = texture2DRect(tex0,gl_TexCoord[0].st);
										   float alpha = color.a;
										   float p = 0.3 *color.g + 0.59*color.r + 0.11*color.b;
										   p = p * brightness;
										   color = vec4(p,p,p,1.0);
										   color = mix( vec4(1.0,1.0,1.0,1.0),color,contrast);
										   
										   gl_FragColor =  vec4(color.r , color.g, color.b, alpha);
									   }
									   );
			
			shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
			shader.linkProgram();
		}
		
		void update(ofFbo& _drawBuffer, ofTexture& _srcTexture, float _contrast, float _brightness){
			_drawBuffer.begin();
			shader.begin();
			shader.setUniformTexture( "tex0" , _srcTexture, 0 );
			shader.setUniform1f("contrast", _contrast);
			shader.setUniform1f("brightness", _brightness);
			renderFrame(_drawBuffer.getWidth(), _drawBuffer.getHeight());
			shader.end();
			_drawBuffer.end();
		}
	};
}