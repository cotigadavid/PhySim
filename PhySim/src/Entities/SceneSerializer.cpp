#include "pspch.h"
#include "SceneSerializer.h"

#include "Entity.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace PhySim {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
		: m_Scene(scene)
	{

	}

	static void SerializeEntity(YAML::Emitter& out, Entity* entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "1135";

		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Name" << YAML::Value << entity->m_Name;

			out << YAML::EndMap;
		}

		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Translation" << YAML::Value << entity->m_Translation;
			out << YAML::Key << "Rotation" << YAML::Value << entity->m_Rotation;
			out << YAML::Key << "Scale" << YAML::Value << entity->m_Scale;

			out << YAML::EndMap;
		}

		Quad* quad = dynamic_cast<Quad*>(entity);

		if (quad)
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Color" << YAML::Value << quad->m_Color;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
	
		for (Entity* entity : m_Scene->m_Entities)
			SerializeEntity(out, entity);

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{

	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		PS_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;

				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Name"].as<std::string>();

				Entity* deserializedEntity = new Entity(name, m_Scene.get());
				Quad* deserializedQuad = nullptr; 

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					deserializedQuad = new Quad(name, m_Scene.get());
					deserializedQuad->m_Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{

					if (deserializedQuad)
					{
						deserializedQuad->m_Translation = transformComponent["Translation"].as<glm::vec3>();
						deserializedQuad->m_Rotation = transformComponent["Rotation"].as<glm::vec3>();
						deserializedQuad->m_Scale = transformComponent["Scale"].as<glm::vec3>();
					}

					else
					{
						deserializedEntity->m_Translation = transformComponent["Translation"].as<glm::vec3>();
						deserializedEntity->m_Rotation = transformComponent["Rotation"].as<glm::vec3>();
						deserializedEntity->m_Scale = transformComponent["Scale"].as<glm::vec3>();
					}
				}

				if (deserializedQuad)
					m_Scene->AddEntity(deserializedQuad);
				else
					m_Scene->AddEntity(deserializedEntity);

				//delete deserializedQuad;
				//delete deserializedEntity;
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		return true;
	}
}