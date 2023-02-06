#include "Rename.hpp"

using namespace std;

void Rename::evaluate(ROB_LINE_S ins_after_decode) {
  ins_after_rename = ins_after_decode;
  if (ins_after_rename.basic_use_rd & ins_after_rename.basic_rd != 0) {
    ins_after_rename.basic_rename_rd = free_list[free_list_head];
    cout << "rd : " << ins_after_rename.basic_rd
         << "rename rd : " << free_list[free_list_head] << endl;
    ins_after_rename.basic_last_rename_rd =
        rename_rd[ins_after_rename.basic_rd];
    // rename_rd[ins_after_rename.basic_rd] = free_list[free_list_head];
    free_list_head = (free_list_head + 1) % FREE_LIST;
  }
}

void Rename::reset() {
  for (int i = 0; i < FREE_LIST; i++) {
    free_list[i] = i + 1;
  }
}