#include "pspch.h"
#include "SceneSerializer.h"

#include "Entity.h"

#include <fstream>

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

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

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

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

	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		PS_ASSERT(false, "Unknown body type");
		return {};
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		PS_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
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

		if (entity->rb2d) 
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap; // Rigidbody2DComponent

			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(entity->rb2d->Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << entity->rb2d->FixedRotation;

			out << YAML::EndMap;
		}

		if (entity->bc2d)
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // BoxCollider2DComponent

			out << YAML::Key << "Offset" << YAML::Value << entity->bc2d->Offset;
			out << YAML::Key << "Size" << YAML::Value << entity->bc2d->Size;
			out << YAML::Key << "Density" << YAML::Value << entity->bc2d->Density;
			out << YAML::Key << "Friction" << YAML::Value << entity->bc2d->Friction;
			out << YAML::Key << "Restitution" << YAML::Value << entity->bc2d->Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << entity->bc2d->RestitutionThreshold;

			out << YAML::EndMap;
		}

		if (entity->spriteComponent)
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Color" << YAML::Value << entity->spriteComponent->m_Color;

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

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					deserializedEntity->spriteComponent = new SpriteComponent();
					deserializedEntity->spriteComponent->m_Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					deserializedEntity->m_Translation = transformComponent["Translation"].as<glm::vec3>();
					deserializedEntity->m_Rotation = transformComponent["Rotation"].as<glm::vec3>();
					deserializedEntity->m_Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					
					deserializedEntity->rb2d->Type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					deserializedEntity->rb2d->FixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
					
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					deserializedEntity->bc2d->Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					deserializedEntity->bc2d->Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					deserializedEntity->bc2d->Density = boxCollider2DComponent["Density"].as<float>();
					deserializedEntity->bc2d->Friction = boxCollider2DComponent["Friction"].as<float>();
					deserializedEntity->bc2d->Restitution = boxCollider2DComponent["Restitution"].as<float>();
					deserializedEntity->bc2d->RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

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