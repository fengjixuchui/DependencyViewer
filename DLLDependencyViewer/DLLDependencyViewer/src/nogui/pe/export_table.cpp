#include "export_table.h"

#include "coff_full.h"
#include "mz.h"
#include "pe_util.h"

#include "../assert.h"


bool pe_parse_export_directory_table(void const* const fd, int const file_size, pe_export_directory_table& edt_out)
{
	char const* const file_data = static_cast<char const*>(fd);
	pe_dos_header const& dos_hdr = *reinterpret_cast<pe_dos_header const*>(file_data + 0);
	pe_coff_full_32_64 const& coff_hdr = *reinterpret_cast<pe_coff_full_32_64 const*>(file_data + dos_hdr.m_pe_offset);
	bool const is_32 = coff_hdr.m_32.m_standard.m_signature == s_pe_coff_optional_sig_32;
	std::uint32_t const dir_tbl_cnt = is_32 ? coff_hdr.m_32.m_windows.m_data_directory_count : coff_hdr.m_64.m_windows.m_data_directory_count;
	if(static_cast<int>(pe_e_directory_table::export_table) >= dir_tbl_cnt)
	{
		edt_out.m_table = nullptr;
		return true;
	}
	pe_data_directory const* const dir_tbl = reinterpret_cast<pe_data_directory const*>(file_data + dos_hdr.m_pe_offset + (is_32 ? sizeof(pe_coff_full_32) : sizeof(pe_coff_full_64)));
	pe_data_directory const& exp_tbl = dir_tbl[static_cast<int>(pe_e_directory_table::export_table)];
	if(exp_tbl.m_va == 0 || exp_tbl.m_size == 0)
	{
		edt_out.m_table = nullptr;
		return true;
	}
	pe_section_header const* sct;
	std::uint32_t const exp_dir_tbl_raw = pe_find_object_in_raw(file_data, file_size, exp_tbl.m_va, exp_tbl.m_size, sct);
	WARN_M_R(exp_dir_tbl_raw != 0, L"Export directory table not found in any section.", false);
	pe_export_directory_entry const* const edt = reinterpret_cast<pe_export_directory_entry const*>(file_data + exp_dir_tbl_raw);
	WARN_M_R(edt->m_ordinal_base <= 0xFFFF, L"Ordinal base is too high.", false);
	WARN_M_R(edt->m_export_address_count <= 0xFFFF, L"Too many addresses to export.", false);
	WARN_M_R(edt->m_ordinal_base + edt->m_export_address_count <= 0xFFFF, L"Biggest ordinal is too high.", false);
	WARN_M_R(edt->m_names_count <= edt->m_export_address_count, L"More names than exported addresses.", false);
	edt_out.m_table = edt;
	return true;
}
