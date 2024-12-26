#include <string>
#include <vector>
#include <ostream>

#include "../utility.h"


namespace Day9
{

    using TVal = std::uint64_t;
    
    struct DiskElem {
        int id; // -1 indicates a free space
        int len;
    };

    std::vector<DiskElem> convert_to_disk_elem(const std::string& disk_map);
    std::vector<DiskElem> compact_hard_drive(const std::vector<DiskElem>& disk_elem_vec);
    TVal calc_check_sum(const std::vector<DiskElem>& disk_elem_vec);
    std::vector<DiskElem> compact_hard_drive_2(std::vector<DiskElem> disk_elem_vec);

    std::ostream& print_elem_vec(std::ostream& out, const std::vector<DiskElem>& elem_vec, bool textual=false);

    TVal sol_9_1(const std::string &file_path)
    {
        std::string disk_map = read_single_str_from_file(file_path);
        auto disk_elem_vec = convert_to_disk_elem(disk_map);
        auto compact_drive = compact_hard_drive(disk_elem_vec);

        auto check_sum = calc_check_sum(compact_drive);
        return check_sum;
    }


    TVal sol_9_2(const std::string &file_path)
    {
        std::string disk_map = read_single_str_from_file(file_path);
        auto disk_elem_vec = convert_to_disk_elem(disk_map);
        auto compact_drive = compact_hard_drive_2(disk_elem_vec);

        auto check_sum = calc_check_sum(compact_drive);
        return check_sum;
    }

    TVal calc_check_sum(const std::vector<DiskElem>& disk_elem_vec)
    {
        TVal check_sum{ 0 };
        TVal idx{ 0 };

        for (size_t i=0; i<disk_elem_vec.size(); ++i) {
            if (disk_elem_vec[i].id == -1) idx += disk_elem_vec[i].len;
            else {
                for (int j=0; j<disk_elem_vec[i].len; ++j) {
                    check_sum += (TVal)disk_elem_vec[i].id * idx;
                    ++idx;
                }
            }
        }

        return check_sum;
    }

    std::vector<DiskElem> compact_hard_drive_2(std::vector<DiskElem> disk_elem_vec)
    {
        std::vector<DiskElem> compact_drive;
        int d_size = disk_elem_vec.size();

        int j = (d_size%2==0) ? d_size-2 : d_size-1;

        for (; j>0; --j)
        {
            auto& to_be_inserted = disk_elem_vec[j];
            if (to_be_inserted.id == -1) continue;

            for (int i=0; i<j; ++i)
            {
                auto& next_elem = disk_elem_vec[i];

                if (next_elem.id != -1) continue;
                else {
                    if (next_elem.len >= to_be_inserted.len) {
                        compact_drive.push_back(to_be_inserted);
                        next_elem.len -= to_be_inserted.len;

                        // update values in place
                        auto tmp = to_be_inserted;
                        to_be_inserted.id = -1;
                        disk_elem_vec.insert(disk_elem_vec.begin()+i, tmp);
                        break;
                    }
                }
            }
        }

        // print_elem_vec(std::cout, disk_elem_vec);

        return disk_elem_vec;
    }

    std::vector<DiskElem> compact_hard_drive(const std::vector<DiskElem>& disk_elem_vec)
    {
        std::vector<DiskElem> compact_drive;
        int d_size = disk_elem_vec.size();

        int j = (d_size%2==0) ? d_size-2 : d_size-1;
        auto to_be_inserted = disk_elem_vec[j];

        for (int i=0; i<j; ++i)
        {
            if (i%2 == 0) compact_drive.push_back(disk_elem_vec[i]);
            else{
                auto free_space = disk_elem_vec[i];
                if (i >= j) break;

                while (free_space.len > 0 && i < j) {

                    if (free_space.len >= to_be_inserted.len) {
                        compact_drive.push_back(to_be_inserted);
                        free_space.len -= to_be_inserted.len;
                        j -= 2;
                        to_be_inserted = disk_elem_vec[j];
                    }
                    else {
                        // fill free space 
                        DiskElem new_elem = to_be_inserted;
                        new_elem.len = free_space.len;
                        compact_drive.push_back(new_elem);

                        // update lengths in free space and file block from which we currently insert
                        to_be_inserted.len -= new_elem.len;
                        free_space.len = 0;
                    }
                }
            }
        }

        // insert remainder of to_be_inserted
        compact_drive.push_back(to_be_inserted);
        // print_elem_vec(std::cout, compact_drive);

        return compact_drive;
    }

    std::vector<DiskElem> convert_to_disk_elem(const std::string& disk_map)
    {
        std::vector<DiskElem> disk_elem_vec;

        int id{ 0 };
        for (int i=0; i<disk_map.size(); ++i) {
            if (i%2 == 0) id = i/2;
            else id = -1;
            disk_elem_vec.push_back(DiskElem{ id,disk_map[i]-'0' });
        }

        return disk_elem_vec;
    }

    std::ostream& print_elem_vec(std::ostream& out, const std::vector<DiskElem>& elem_vec, bool textual)
    {
        for (const auto& elem : elem_vec) {
            if (elem.id == -1) {
                for (int i=0; i<elem.len; ++i) out << '.';
            }
            else {
                for (int i=0; i<elem.len; ++i) out << elem.id;
            }
        }

        out << "\n\n";
        return out;
    }

} // namespace Day9
