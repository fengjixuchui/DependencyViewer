#pragma once


#include "../nogui/manifest_parser.h"
#include "../nogui/memory_manager.h"
#include "../nogui/pe.h"
#include "../nogui/my_vector.h"

#include <filesystem>
#include <string>


struct file_info;
typedef my_vector<file_info> file_infos;


struct file_info
{
	void* m_tree_item = nullptr;
	file_info* m_orig_instance = nullptr;
	wstring const* m_file_path = nullptr;
	bool m_is_32_bit = true;
	pe_import_table_info m_import_table;
	pe_export_table_info m_export_table;
	std::uint16_t* m_matched_imports;
	file_infos m_sub_file_infos;
	pe_resources_table_info m_resources_table;
	manifest_data m_manifest_data;
};

struct main_type
{
	memory_manager m_mm;
	file_info m_fi;
	std::string m_tmpn;
	std::wstring m_tmpw;
	std::filesystem::path m_tmpp;
};

wstring const* get_not_found_string();
main_type process(std::wstring const& main_file_path);
