//#ifndef __GL_UTILS_HPP__
//#define __GL_UTILS_HPP__
//
//#include "GL/gl3w.h"
//#include <cstdlib>
//#include <exception>
//#include <iostream>
//#include <string>
//
//
//	static void APIENTRY glDebugMessage( const GLenum	p_source,
//										 const GLenum	p_type,
//										 const GLuint	p_id,
//										 const GLenum	p_severity,
//										 const GLsizei	p_length,
//										 const GLchar * p_msg,
//										 const void *	p_data )
//	{
//		std::string source;
//		std::string type;
//		std::string severity;
//
//		switch ( p_source )
//		{
//		case GL_DEBUG_SOURCE_API: source = "API"; break;
//		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: source = "WINDOW SYSTEM"; break;
//		case GL_DEBUG_SOURCE_SHADER_COMPILER: source = "SHADER COMPILER"; break;
//		case GL_DEBUG_SOURCE_THIRD_PARTY: source = "THIRD PARTY"; break;
//		case GL_DEBUG_SOURCE_APPLICATION: source = "APPLICATION"; break;
//		case GL_DEBUG_SOURCE_OTHER: source = "UNKNOWN"; break;
//		default: source = "UNKNOWN"; break;
//		}
//
//		switch ( p_type )
//		{
//		case GL_DEBUG_TYPE_ERROR: type = "ERROR"; break;
//		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type = "DEPRECATED BEHAVIOR"; break;
//		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: type = "UDEFINED BEHAVIOR"; break;
//		case GL_DEBUG_TYPE_PORTABILITY: type = "PORTABILITY"; break;
//		case GL_DEBUG_TYPE_PERFORMANCE: type = "PERFORMANCE"; break;
//		case GL_DEBUG_TYPE_OTHER: type = "OTHER"; break;
//		case GL_DEBUG_TYPE_MARKER: type = "MARKER"; break;
//		default: type = "UNKNOWN"; break;
//		}
//
//		switch ( p_severity )
//		{
//		case GL_DEBUG_SEVERITY_HIGH: severity = "HIGH"; break;
//		case GL_DEBUG_SEVERITY_MEDIUM: severity = "MEDIUM"; break;
//		case GL_DEBUG_SEVERITY_LOW: severity = "LOW"; break;
//		// GL_DEBUG_SEVERITY_NOTIFICATION should not happen.
//		case GL_DEBUG_SEVERITY_NOTIFICATION: severity = "NOTIFICATION"; break;
//		default: severity = "UNKNOWN"; break;
//		}
//
//		std::string message( "[OPENGL] [" + severity + "] [" + type + "] " + source + ": " + p_msg );
//
//		std::cerr << message << std::endl;
//	}
//
//#endif //__GL_UTILS_HPP__
