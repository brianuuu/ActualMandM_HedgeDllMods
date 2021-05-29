#include "ArchiveTreePatcher.h"

vector<ArchiveDependency> ArchiveTreePatcher::m_archiveDependencies =
{
	{ "ColorsSonic", {"ev031", "ev041", "ev042", "ev091", "evSonic", "Title"}}
};

HOOK(bool, __stdcall, ParseArchiveTree, 0xD4C8E0, void* A1, char* pData, const size_t size, void* pDatabase)
{
	std::string str;
	{
		std::stringstream stream;

		for (ArchiveDependency const& node : ArchiveTreePatcher::m_archiveDependencies)
		{
			stream << "  <Node>\n";
			stream << "    <Name>" << node.m_archive << "</Name>\n";
			stream << "    <Archive>" << node.m_archive << "</Archive>\n";
			stream << "    <Order>" << 0 << "</Order>\n";
			stream << "    <DefAppend>" << node.m_archive << "</DefAppend>\n";

			for (string const& dependency : node.m_dependencies)
			{
				stream << "    <Node>\n";
				stream << "      <Name>" << dependency << "</Name>\n";
				stream << "      <Archive>" << dependency << "</Archive>\n";
				stream << "      <Order>" << 0 << "</Order>\n";
				stream << "    </Node>\n";
			}

			stream << "  </Node>\n";
		}

		str = stream.str();
	}

	const size_t newSize = size + str.size();
	const std::unique_ptr<char[]> pBuffer = std::make_unique<char[]>(newSize);
	memcpy(pBuffer.get(), pData, size);

	char* pInsertionPos = strstr(pBuffer.get(), "<Include>");

	memmove(pInsertionPos + str.size(), pInsertionPos, size - (size_t)(pInsertionPos - pBuffer.get()));
	memcpy(pInsertionPos, str.c_str(), str.size());

	bool result;
	{
		result = originalParseArchiveTree(A1, pBuffer.get(), newSize, pDatabase);
	}

	return result;
}

bool ArchiveTreePatcher::enabled = false;

void ArchiveTreePatcher::applyPatches()
{
	if (enabled)
		return;

	enabled = true;

	INSTALL_HOOK(ParseArchiveTree);
}
