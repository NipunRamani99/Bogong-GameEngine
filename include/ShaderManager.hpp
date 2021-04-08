#pragma once
#include "Shaders.hpp"
#include <unordered_map>
#include <algorithm>
#include "Globals.h"
namespace bogong {
	
	/*


		shader_source.open(PS,"shaders/ObjectFragmentShader.glsl");
		shader_source.addMacro("HAS_UV");
		vertex_source.open(VS,"shaders/ObjectVertexShader.glsl");
		vertex_source.addMacro("HAS_UV");
		std::vector<ShaderSource> shaders;
		shaders.push_back(shader_source);
		shaders.push_back(vertex_source);
		int prog = ProgramManager::createProgram(shaders);
		...
     	...
		renderer.addShaderProgram(program);
	*/
	
	
	struct Configuration {
		std::vector<std::string> macros;
		bool hasGeometryShader = false;
		bool hasHullShader = false;

	};
	class Program {
	private:
		unsigned int m_ProgID;
		unsigned int m_VertID;
		unsigned int m_FragID;
		unsigned int m_GeomID;
	public:
		Program()
			:
			m_ProgID(0),
			m_VertID(0),
			m_FragID(0),
			m_GeomID(0)
		{

		}
		void operator=(Program program) {
			m_ProgID = program.m_ProgID;
			m_FragID = program.m_FragID;
			m_VertID = program.m_VertID;
			m_GeomID = program.m_GeomID;
		}
		void LoadProgram()
		{
			
			std::cout << "Linking the program.\n";
			CHECK_GL_ERROR(m_ProgID = glCreateProgram());
			if (m_VertID != 0)
				CHECK_GL_ERROR(glAttachShader(m_ProgID, m_VertID));
			if (m_FragID != 0)
				CHECK_GL_ERROR(glAttachShader(m_ProgID, m_FragID));
			if (m_GeomID != 0)
				CHECK_GL_ERROR(glAttachShader(m_ProgID, m_GeomID));

			CHECK_GL_ERROR(glLinkProgram(m_ProgID));

			GLint l_Result = 0;
			GLint l_LogLength = 0;
			CHECK_GL_ERROR(glGetProgramiv(m_ProgID, GL_LINK_STATUS, &l_Result));
			CHECK_GL_ERROR(glGetProgramiv(m_ProgID, GL_INFO_LOG_LENGTH, &l_LogLength));

			if (l_LogLength > 0)
			{
				std::vector<char> error(l_LogLength + 1);
			CHECK_GL_ERROR(	glGetProgramInfoLog(m_ProgID, l_LogLength, NULL, &error[0]) );
				std::cout << (std::string(&error[0]));
			}

			if (m_VertID != 0)
			CHECK_GL_ERROR(	glDetachShader(m_ProgID, m_VertID) );
			if (m_FragID != 0)
			CHECK_GL_ERROR(	glDetachShader(m_ProgID, m_FragID) );
			if (m_GeomID != 0)
            CHECK_GL_ERROR( glDetachShader(m_ProgID, m_GeomID) );
		}
		void LoadShader(const char * p_Path, ShaderType p_Type,std::vector<std::string> & macros)
		{
			std::ifstream l_InFileStream(p_Path, std::ios::in);
			std::stringstream l_SS;
			if (l_InFileStream.is_open())
			{
				l_SS << l_InFileStream.rdbuf();
				l_InFileStream.close();
			}
			else
			{
				std::cout << "Error Loading Shaders: Cannot open file stream.";
				return;
			}
			std::string defs;
			for (auto & macro : macros) {
				defs += "#define " + macro +"\r\n";
			}
			std::string l_Src = l_SS.str();
			defs = "#version 330 core\r\n" + defs;
			l_Src = defs + l_Src;
			const char * l_ShaderSrc = l_Src.c_str();
			unsigned int * l_ID = nullptr;
			std::string shader = " ";
			switch (p_Type)
			{
			case ShaderType::FRAGMENT:
				shader = "Fragment: ";
				m_FragID = glCreateShader(GL_FRAGMENT_SHADER);
				l_ID = &m_FragID;
				break;
			case ShaderType::GEOMETRY:
				shader = "Geometry: ";
				m_GeomID = glCreateShader(GL_GEOMETRY_SHADER);
				l_ID = &m_GeomID;
				break;
			case ShaderType::VERTEX:
				shader = "Vertex: ";
				m_VertID = glCreateShader(GL_VERTEX_SHADER);
				l_ID = &m_VertID;
				break;
			}
			GLint l_Result = GL_FALSE;
			GLint l_LogLength = 0;
			glShaderSource(*l_ID, 1, &l_ShaderSrc, NULL);
			glCompileShader(*l_ID);
			glGetShaderiv(*l_ID, GL_COMPILE_STATUS, &l_Result);
			glGetShaderiv(*l_ID, GL_INFO_LOG_LENGTH, &l_LogLength);

			if (l_LogLength > 0)
			{
				std::cout << "In " << shader;
				std::vector<char> error(l_LogLength + 1);
				glGetShaderInfoLog(*l_ID, l_LogLength, NULL, &error[0]);
				std::cout << (std::string(&error[0]));
			}
			else
			{
				std::cout << "Shader created..\n";
			}
		}
		unsigned int GetProgramID() const
		{
			return m_ProgID;
		}
		void Bind()
		{
			CHECK_GL_ERROR(glUseProgram(m_ProgID));
		}
		inline unsigned int GetLocation(std::string p_Variable)
		{
			CHECK_GL_ERROR(glGetUniformLocation(m_ProgID, p_Variable.c_str()));
		}
		inline void setBool(const std::string &name, bool value) const
		{
			CHECK_GL_ERROR(glUniform1i(glGetUniformLocation(m_ProgID, name.c_str()), (int)value));
		}
		void setInt(const std::string &name, int value) const
		{
			CHECK_GL_ERROR(glUniform1i(glGetUniformLocation(m_ProgID, name.c_str()), value));
		}
		void setFloat(const std::string &name, float value) const
		{
			CHECK_GL_ERROR(glUniform1f(glGetUniformLocation(m_ProgID, name.c_str()), value));
		}
		void setVec2(const std::string &name, const glm::vec2 &value) const
		{
			CHECK_GL_ERROR(glUniform2fv(glGetUniformLocation(m_ProgID, name.c_str()), 1, &value[0]));
		}
		inline void setVec2(const std::string &name, float x, float y) const
		{
			CHECK_GL_ERROR(glUniform2f(glGetUniformLocation(m_ProgID, name.c_str()), x, y));
		}
		inline void setVec3(const std::string &name, const glm::vec3 &value) const
		{
			CHECK_GL_ERROR(glUniform3fv(glGetUniformLocation(m_ProgID, name.c_str()), 1, &value[0]));
		}
		inline void setVec3(const std::string &name, float x, float y, float z) const
		{
			CHECK_GL_ERROR(glUniform3f(glGetUniformLocation(m_ProgID, name.c_str()), x, y, z));
		}
		inline void setVec4(const std::string &name, const glm::vec4 &value) const
		{
			CHECK_GL_ERROR(glUniform4fv(glGetUniformLocation(m_ProgID, name.c_str()), 1, &value[0]));
		}
		void setVec4(const std::string &name, float x, float y, float z, float w)
		{
			CHECK_GL_ERROR(glUniform4f(glGetUniformLocation(m_ProgID, name.c_str()), x, y, z, w));
		}
		void setMat2(const std::string &name, const glm::mat2 &mat) const
		{
		    CHECK_GL_ERROR(glUniformMatrix2fv(glGetUniformLocation(m_ProgID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
		}
		void setMat3(const std::string &name, const glm::mat3 &mat) const
		{
			CHECK_GL_ERROR(glUniformMatrix3fv(glGetUniformLocation(m_ProgID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
		}
		void setMat4(const std::string &name, const glm::mat4 &mat) const
		{
			CHECK_GL_ERROR(glUniformMatrix4fv(glGetUniformLocation(m_ProgID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
		}
		unsigned int GetShader(ShaderType p_Type) const
		{
			unsigned int l_RetnVal = -1;
			switch (p_Type)
			{
			case ShaderType::FRAGMENT:
				l_RetnVal = m_FragID;
				break;
			case ShaderType::GEOMETRY:
				l_RetnVal = m_GeomID;
				break;
			case ShaderType::VERTEX:
				l_RetnVal = m_VertID;
				break;
			}
			return l_RetnVal;
		}
	};
	//singleton this
	class ShaderManager {
		
			static std::unordered_map<size_t,Program> shaders;
			
		
	public:
			static size_t HashShader(std::string name, Configuration configuration) {
				std::hash<std::string> hasher;
				std::string str = name;
				std::sort(configuration.macros.begin(), configuration.macros.end());
				for (auto s : configuration.macros) {
					str += s;
				}
				size_t h = hasher(str);
				return h;
			}
			static void LoadProgram(std::string name, Configuration configuration) {
				Program program;
				std::string file_name = name;
				std::string path_vs = "Shaders/" + name + "VertexShader.glsl";
				std::string path_fs = "Shaders/" + name + "FragmentShader.glsl";
				program.LoadShader(path_vs.c_str(), ShaderType::VERTEX,configuration.macros);
				program.LoadShader(path_fs.c_str(), ShaderType::FRAGMENT,configuration.macros);
				if (configuration.hasGeometryShader) {
					std::string path_gs = "Shaders/" + name + "GeometryShader.glsl";
					program.LoadShader(path_gs.c_str(), ShaderType::GEOMETRY,configuration.macros);
				}
				program.LoadProgram();
				size_t h = HashShader(name, configuration);
				shaders[h] = program;
				
			}
			static Program GetShader(std::string name, Configuration configuration){
			   size_t h = HashShader(name, configuration);
			   if (shaders.find(h) != shaders.end()) {
				   return shaders.at(h);
			   }
			   else
			   {
				   LoadProgram(name,configuration);
				   return shaders[h];
			   }
			}
		
	};
}