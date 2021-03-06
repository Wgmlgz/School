#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <windows.h> // for color text
using namespace std;

class Table {
 public:
  size_t vars, num;
  size_t table_length;
  vector<vector<vector<int>>> table;
  vector<vector<vector<int>>> colors;
  vector<vector<int>> helper;
  string true_ans;
  vector<string> ans;
  vector<int> toBin(int val, int size) {
    vector<int> ret(size);
    int j = 0;
    for (int i = size - 1; i >= 0; --i) {
      ret[j] = (bool)((val >> i) & 1);
      ++j;
    }
    return ret;
  };
  int sumVec(vector<int> v) {
    int res = 0;
    for (auto i : v) res += i;
    return res;
  }
  void setDefaultColors() {
      colors = table;
      for (int i = 0; i < table_length + 1; ++i)
          for (int t = 0; t < table.size(); ++t)
              for (int j = 0; j < table[t][i].size(); ++j)
                  if (table[t][i][j] < 0) colors[t][i][j] = 9;
                  else colors[t][i][j] = colors[t][i][j] ? 7 : 8;
  }
  void createDefaultTable(size_t vars) {
    table.clear();
    colors.clear();
    helper.clear();
    ans.clear();
    true_ans.clear();

    table_length = 1 << vars;
    helper.resize(table_length + 1);
    helper[0].resize(vars);
    for (int i = 1; i <= vars; ++i) helper[0][i - 1] = -i;
    for (int i = 1; i <= table_length; ++i) {
      helper[i] = toBin(i - 1, vars);
    }
    for (int i = 1; i <= vars; ++i) {
      for (int j = helper.size() - 1; j > 0; --j) {
        if (sumVec(helper[j]) == i) {
          auto cop = helper;
          int rem = 0;
          for (int k = 0; k < helper[j].size(); ++k) {
            if (helper[j][k] == 0) {
              for (auto& row : cop) {
                row.erase(row.begin() + (k - rem));
              }
              ++rem;
            }
          }
          table.push_back(cop);
        }
      }
    }
    setDefaultColors();
  }
  void removeZeroRows(int number) {
    auto bin = toBin(number, table_length);
    for (int i = 1; i <= table_length; ++i) {
      if (bin[i - 1] == 0) {
        for (int t = 0; t < table.size(); ++t)
          for (int j = 0; j < table[t][i].size(); ++j)
            colors[t][i][j] = 71;
      }
    }
  }
  void removeExistingInZeroRows() {
    for (int t = 0; t < table.size(); ++t) {
      for (int i = 1; i < table_length + 1; ++i) {
        for (int j = 1; j < table_length + 1; ++j) {
          if (colors[t][j][0] == 71 && table[t][j] == table[t][i] && colors[t][i][0] != 71) {
            for (auto& k : colors[t][i]) k = 67;
          }
        }
      }
    }
  }
  void absorptionInString() {
    for (int i = 1; i < table_length + 1; ++i) {
      for (int t = 0; t < table.size(); ++t) {
        if (colors[t][i][0] < 9) {
          for (int j = t + 1; j < table.size(); ++j) {
            if (colors[t][i][0] < 9) {
              bool b = true;
              for (auto f : table[t][0])
                if (find(table[j][0].begin(), table[j][0].end(), f) ==
                    table[j][0].end()) {
                  b = false;
                }
              if (b) for (auto& f : colors[j][i]) f = 74;
            }
          }
        }
      }
    }
  }
  void removeBadCols() {
    for (int i = 0; i < table.size(); ++i) {
      bool b = true;
      for (auto j : colors[i])
        if (j[0] < 9) b = false;
      if (b) {
        table.erase(table.begin() + i);
        colors.erase(colors.begin() + i);
        --i;
      }
    }
  }
  string vecToLet(int a, int b) {
    string res;
    for (int f = 0; f < colors[a][b].size(); ++f) {
      res += (table[a][b][f]? "" : "!");
      res += (char)((-table[a][0][f] - 1) + 'a');
    }
    return res;
  }
  void findGood() {
    for (int i = 1; i < table_length + 1; ++i) {
      int ans = 0;
      int index;
      for (int t = 0; t < table.size(); ++t) {
        if (colors[t][i][0] < 9) {
          ++ans;
          index = t;
        }
      }
      if (ans == 1) {
        true_ans += vecToLet(index, i);
        true_ans += " ";
        for (auto& f : colors[index][i]) f = 39;
        for (int j = 1; j < table_length + 1; ++j) {
          if (table[index][j] == table[index][i] && i != j) {
            for (int k = 0; k < table.size(); ++k) {
              for (auto& f : colors[k][j]) f = 192 + f % 16;
            }
          }
        }
        //table.erase(table.begin() + index);
        //colors.erase(colors.begin() + index);
      }
    }
  }
  void clearStuff() {
    //int len = 1 << table.size();
    //for (int i = 0; i < len; ++i) {
    //  auto da = toBin(i, table.size());
    //  vector<int> res(table_length, 0);
    //  vector<int> index(table.size(), 0);
    //  for (int j = 0; j < table.size(); ++j) {
    //    if (da[j]) {
    //      for (int k = 1; k < table_length + 1; ++k) {
    //        if (colors[j][k][0] < 9) {
    //          if (res[k - 1] == 0) {
    //            res[k - 1] = 1;
    //            index[j] = k;
    //          }
    //        }
    //      }
    //    }
    //  }
    //  if (sumVec(res) == table_length - 1) {
    //    string res;
    //    for (int j = 0; j < table.size(); ++j) {
    //      if (da[j]) {
    //        //res += vecToLet(index[])
    //      }
    //    }
    //  }
    //  for (auto j : da) std::cout << j << " ";
    //  std::cout << std::endl;
    //}
    for (int i = 1; i < table_length + 1; ++i) {
      bool b = true;
      for (int t = 0; t < table.size(); ++t)
        if (colors[t][i][0] < 9) b = false;
      if (b) {
        for (auto& j : table) j.erase(j.begin() + i);
        for (auto& j : colors) j.erase(j.begin() + i);
        --table_length;
        --i;
      }
    }
    removeBadCols();
    //for (int i = 1)
  }
  void parceAns() {
    vector<pair<int, int>> v;
    parceAnsRec(v, 1);
  }
  void parceAnsRec(vector<pair<int, int>> v, int index) {
    if (index == table_length + 1) {
      string res = true_ans;
      for (auto i : v) {
        res += vecToLet(i.first, i.second);
        res += " ";
      }

      ans.push_back(res);
      return;
    }
    for (int i = 0; i < table.size(); ++i) {
      if (colors[i][index][0] < 9) {
        bool b = true;
        for (auto j = v.begin(); j < v.end(); ++j) {
          if (colors[j->first][j->second] == colors[i][index] && i == j->first) {
            b = false;
          }
        }
        auto c = v;
        if (b) {
          c.push_back(make_pair(i, index));
        }
        parceAnsRec(c, index + 1);
      }
    }
  }
  void removeLong() {
    int min = 666;
    for (int i = 0; i < ans.size(); ++i) {
      int t = 0;
      for (auto j : ans[i])
        if (j >= 'a' && j <= 'z') ++t;
      if (t < min) min = t;
    }
    for (int i = 0; i < ans.size(); ++i) {
      int t = 0;
      for (auto j : ans[i])
        if (j >= 'a' && j <= 'z') ++t;
      if (t > min) {
        ans.erase(ans.begin() + i);
        --i;
      }
    }
  }

  void calc(size_t nvars, size_t nnum, int step) {
      vars = nvars;
      num = nnum;
      // 012345678901230123456789 012345678901230123456789 012345678901230123456789 012345678901230123456789 012345678901230123456789
      // default table | removing zero rows | remove cells that existing in zero rows | absorption in strings | remove bad columns | 
      if (step >= 0) createDefaultTable(vars);
      if (step >= 1) removeZeroRows(num);
      if (step >= 2) removeExistingInZeroRows();
      if (step >= 3) absorptionInString();
      if (step >= 4) removeBadCols();
      if (step >= 5) findGood();
      if (step >= 6) clearStuff();
      if (step >= 7) parceAns();
      if (step >= 8) removeLong();
  }

  Table(size_t nvars, size_t nnum) {
    calc(nvars, nnum, 8);
  }
  
  void printTableLong(int size) {

  }
  void printTable() {
    for (int i = 0; i < table_length + 1; ++i) {
      for (int t = 0; t < table.size(); ++t) {
        for (int j = 0; j < table[t][i].size(); ++j) {
          HANDLE hConsole;
          hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
          SetConsoleTextAttribute(hConsole, colors[t][i][j]);
          if (table[t][i][j] < 0) {
            cout << (char)((-table[t][i][j] - 1) + 'a');
          } else {
            cout << std::to_string(table[t][i][j]);
          }
          cout << " ";
          SetConsoleTextAttribute(hConsole, 7);
        }
        cout << " ";
      }
      cout << "'" << endl;
    }
    cout << endl;
  }
  std::pair<std::vector<std::vector<std::pair<std::string, int>>>, int> genTable(int vars, int num, int step) {
      calc(vars, num, step);
      std::vector<std::vector<std::pair<std::string, int>>> vec;
      int length = 0;
      for (int i = 0; i < table_length + 1; ++i) {
          std::vector<std::pair<std::string, int>> tmp;
          for (int t = 0; t < table.size(); ++t) {
              std::pair<std::string, int> tmp_cell;
              for (int j = 0; j < table[t][i].size(); ++j) {
                  if (i == 0) ++length;
                  if (table[t][i][j] < 0) {
                      std::string s;
                      s.push_back((-table[t][i][j] - 1) + 'a');
                      tmp_cell.first += s;
                  }
                  else {
                      tmp_cell.first += std::to_string(table[t][i][j]);
                  }
                  tmp_cell.second = colors[t][i][j];
              }
              //std::cout << tmp_cell.first << " ";
              tmp.push_back(tmp_cell);
              //tmp.push_back(make_pair(" ", 0));
          }
          //std::cout << std::endl;
          vec.push_back(tmp);
      }
      
      //cout << endl;
      return std::make_pair(vec, length);
  }
  void printAns() {
    for (auto i : ans) std::cout << i << std::endl;
  }
  std::string genAns() {
      std::string res;
      for (auto i : ans) {
          res += i;
          res += "\n";
      }
      return res;
  }
};
