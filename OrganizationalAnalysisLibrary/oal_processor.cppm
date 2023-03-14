export module xablau.organizational_analysis_c_sharp;

export import <cstdint>;

export import xablau.organizational_analysis;

using ResizeContainer = void(*)(size_t size);
using InsertStringIntoPosition1 = void(*)(const char *string, size_t i);
using InsertPairOfStringAndSizeT = void(*)(const char *string, size_t i);
using InsertStringInContainer = void(*)(const char *string);
using ResizeMatrix = void(*)(size_t rows, size_t columns);
using InsertFloatIntoPosition2 = void(*)(float value, size_t i, size_t j);

extern "C"
{
	_declspec(dllexport) uintptr_t construct()
	{
		return
			reinterpret_cast < uintptr_t > (
				new xablau::organizational_analysis::processor < char, std::char_traits < char > > ());
	}

	_declspec(dllexport) void destroy(uintptr_t address)
	{
		delete reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);
	}

	_declspec(dllexport) const char *insert_or_assign_agent(
		const uintptr_t address,
		const char *agent,
		const char *group,
		const char *role)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->insert_or_assign_agent(agent, group, role);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *erase_agent(
		uintptr_t address,
		const char *agent)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->erase_agent(agent);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *insert_or_edit_activity(
		uintptr_t address,
		const char *activity,
		const char *name,
		const char *group)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->insert_or_edit_activity(activity, name, group);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *erase_activity(
		uintptr_t address,
		const char *activity)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->erase_activity(activity);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *insert_agent_in_charge_of_activity(
		uintptr_t address,
		const char *agent,
		const char *activity)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->insert_agent_in_charge_of_activity(agent, activity);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *erase_agent_in_charge_of_activity(
		uintptr_t address,
		const char *agent,
		const char *activity)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->erase_agent_in_charge_of_activity(agent, activity);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *insert_activity_dependency(
		uintptr_t address,
		const char *dependent,
		const char *dependency)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->insert_activity_dependency(dependent, dependency);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *erase_activity_dependency(
		uintptr_t address,
		const char *dependent,
		const char *dependency)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->erase_activity_dependency(dependent, dependency);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *insert_or_edit_component(
		uintptr_t address,
		const char *component,
		const char *name,
		const char *group)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->insert_or_edit_component(component, name, group);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *erase_component(
		uintptr_t address,
		const char *component)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->erase_component(component);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *insert_component_interface(
		uintptr_t address,
		const char *component1,
		const char *component2)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->insert_component_interface(component1, component2);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *erase_component_interface(
		uintptr_t address,
		const char *component1,
		const char *component2)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->erase_component_interface(component1, component2);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *insert_or_assign_affiliation(
		uintptr_t address,
		const char *activity,
		const char *component,
		const float rating)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->insert_or_assign_affiliation(activity, component, rating);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *erase_affiliation(
		uintptr_t address,
		const char *activity,
		const char *component)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->erase_affiliation(activity, component);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) void clear(uintptr_t address)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		processor->clear();
	}

	_declspec(dllexport) const char *identify_parallelizations(
		uintptr_t address,
		ResizeContainer resizer,
		InsertStringIntoPosition1 inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto parallelizations = processor->identify_parallelizations();

			resizer(parallelizations.size());

			for (size_t i = 0; i < parallelizations.size(); i++)
			{
				for (const auto &activity : parallelizations[i])
				{
					inserter(activity.c_str(), i);
				}
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *identify_priorities(
		uintptr_t address,
		InsertPairOfStringAndSizeT inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto priorities = processor->identify_priorities();

			for (const auto &[activity, priority] : priorities)
			{
				inserter(activity.c_str(), priority);
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *validate_agents_in_charge_for_components(
		uintptr_t address,
		InsertStringInContainer inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto componentsWithoutAgents = processor->validate_agents_in_charge_for_components();

			for (const auto &componentWithoutAgents : componentsWithoutAgents)
			{
				inserter(componentWithoutAgents.c_str());
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *attribute_agents_in_charge_for_components(
		uintptr_t address,
		float minimumRelationDegree)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->attribute_agents_in_charge_for_components(minimumRelationDegree);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *align_architecture_process_between_components_and_organization(
		uintptr_t address,
		float minimumRelationDegree)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->align_architecture_process_between_components_and_organization(minimumRelationDegree);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *align_architecture_process_between_activities_and_organization(
		uintptr_t address,
		float minimumRelationDegree)
	{
		auto processor =
			reinterpret_cast < xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			processor->align_architecture_process_between_activities_and_organization(minimumRelationDegree);
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *activities_dependencies_matrix(
		uintptr_t address,
		ResizeMatrix resizer,
		InsertFloatIntoPosition2 inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto &activities_dependencies_matrix = processor->activities_dependencies_matrix();

			resizer(activities_dependencies_matrix.rows(), activities_dependencies_matrix.columns());

			for (size_t i = 0; i < activities_dependencies_matrix.rows(); i++)
			{
				for (size_t j = 0; j < activities_dependencies_matrix.columns(); j++)
				{
					inserter(activities_dependencies_matrix(i, j), i, j);
				}
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *components_interfaces_matrix(
		uintptr_t address,
		ResizeMatrix resizer,
		InsertFloatIntoPosition2 inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto &components_interfaces_matrix = processor->components_interfaces_matrix();

			resizer(components_interfaces_matrix.rows(), components_interfaces_matrix.columns());

			for (size_t i = 0; i < components_interfaces_matrix.rows(); i++)
			{
				for (size_t j = 0; j < components_interfaces_matrix.columns(); j++)
				{
					inserter(components_interfaces_matrix(i, j), i, j);
				}
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *weak_affiliations_matrix(
		uintptr_t address,
		ResizeMatrix resizer,
		InsertFloatIntoPosition2 inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto &affiliations_matrix = processor->weak_affiliations_matrix();

			resizer(affiliations_matrix.rows(), affiliations_matrix.columns());

			for (size_t i = 0; i < affiliations_matrix.rows(); i++)
			{
				for (size_t j = 0; j < affiliations_matrix.columns(); j++)
				{
					inserter(affiliations_matrix(i, j), i, j);
				}
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *strong_affiliations_matrix(
		uintptr_t address,
		ResizeMatrix resizer,
		InsertFloatIntoPosition2 inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto &affiliations_matrix = processor->strong_affiliations_matrix();

			resizer(affiliations_matrix.rows(), affiliations_matrix.columns());

			for (size_t i = 0; i < affiliations_matrix.rows(); i++)
			{
				for (size_t j = 0; j < affiliations_matrix.columns(); j++)
				{
					inserter(affiliations_matrix(i, j), i, j);
				}
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *comparative_matrix_without_redundancies(
		uintptr_t address,
		ResizeMatrix resizer,
		InsertFloatIntoPosition2 inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto &comparative_matrix_without_redundancies = processor->comparative_matrix_without_redundancies();

			resizer(comparative_matrix_without_redundancies.rows(), comparative_matrix_without_redundancies.columns());

			for (size_t i = 0; i < comparative_matrix_without_redundancies.rows(); i++)
			{
				for (size_t j = 0; j < comparative_matrix_without_redundancies.columns(); j++)
				{
					inserter(comparative_matrix_without_redundancies(i, j), i, j);
				}
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}

	_declspec(dllexport) const char *comparative_matrix_with_redundancies(
		uintptr_t address,
		ResizeMatrix resizer,
		InsertFloatIntoPosition2 inserter)
	{
		auto processor =
			reinterpret_cast < const xablau::organizational_analysis::processor < char, std::char_traits < char > > * > (address);

		try
		{
			const auto &comparative_matrix_with_redundancies = processor->comparative_matrix_with_redundancies();

			resizer(comparative_matrix_with_redundancies.rows(), comparative_matrix_with_redundancies.columns());

			for (size_t i = 0; i < comparative_matrix_with_redundancies.rows(); i++)
			{
				for (size_t j = 0; j < comparative_matrix_with_redundancies.columns(); j++)
				{
					inserter(comparative_matrix_with_redundancies(i, j), i, j);
				}
			}
		}
		catch (const std::exception &exception)
		{
			static std::string message;

			message = exception.what();

			return message.c_str();
		}

		return nullptr;
	}
}