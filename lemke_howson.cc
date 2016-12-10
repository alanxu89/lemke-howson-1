// Lemke-Howson Algorithm - with heuristic
// Based on paper: https://arxiv.org/abs/0811.3247
// Code made by: Marcelo Gomes and Supervised by Guilherme Leobas and Mariana Oliveira
//
#include <bits/stdc++.h>
//#include <vector>
//#include <algorithm>
//#include <iostream>
//#include <string>
//#include <random>

using namespace std;

class tableau {
private:
  vector<vector<double> > matrix;
  vector<string> sVars,xVars; //x1,x2,x3,... //s1,s2,s3,...

  bool checkVar(string var, vector<string> v, int &index) {
    for(int i = 0 ; i < v.size() ; i++) {
      if(var == v[i]) {
        index = i;
        return true;
      }
    }

    index = -1;
    return false;
  }

  bool checkVar(string var, vector<string> v) {
    int useless;
    return checkVar(var, v, useless);
  }

  tableau() {
    
  }

public:

  tableau(vector<vector<int> > payoffMatrix, bool isPlayer1) {
    int rows = payoffMatrix.size();
    int columns = payoffMatrix[0].size();

    // cout << "rows: " << rows << endl;
    // cout << "columns: " << columns << endl;

    if(isPlayer1) {
      matrix.resize(columns);

      for(int i = 1 ; i <= rows ; i++) {
        xVars.push_back("x" + to_string(i));  
      }

      for(int i = rows + 1 ; i < columns + rows + 1 ; i++) {
        sVars.push_back("s" + to_string(i));
      }

      for(int i = rows ; i < matrix.size() + rows; i++) {
        matrix[i-rows].push_back(-(i+1));
        matrix[i-rows].push_back(1);

        for(int j = 0 ; j < columns ; j++ ) {
          matrix[i-rows].push_back(0);
        }

        for(int j = 0 ; j < rows ; j++) {
          matrix[i-rows].push_back(-payoffMatrix[j][i-rows]);
        }
      }

      


    } else {
      matrix.resize(rows);

      for(int i = 1 ; i <= rows ; i++) {
        sVars.push_back("s" + to_string(i));
      }

      for(int i = rows + 1 ; i < columns + rows + 1 ; i++) {
        xVars.push_back("x" + to_string(i));  
      }


      for(int i = 0 ; i < matrix.size(); i++) {
        matrix[i].push_back(-(i+1));
        matrix[i].push_back(1);

        for(int j = 0 ; j < rows ; j++ ) {
          matrix[i].push_back(0);
        }

        for(int j = 0 ; j < columns ; j++) {
          matrix[i].push_back(-payoffMatrix[i][j]);
        }
      }
    }
  }

  bool isMember(string var) {
    return checkVar(var, xVars) || checkVar (var, sVars);
  }

  int getColumn(string var) {
    int index=0;

    if(checkVar(var,xVars,index)) {
      return index + sVars.size() + 2;
    } else if(checkVar(var,sVars,index)){
      return index + 2;
    }

    //this should not happen, howevah...
    cout << "var unknown " << var << endl;
    exit(1);
  }


  vector<pair<string,double> > getEquilibrium() {
    vector<pair<string,double> > result;
    double sum = 0.0;

    cout << "equilibrium: \n";

    for(int i = 0 ; i < matrix.size(); i++ ) {
      if(matrix[i][0] > 0) {
        result.push_back( make_pair("x"+to_string((int)matrix[i][0]), matrix[i][1]));
        sum += matrix[i][1];
      }
    }

    for (int i=0; i<result.size(); i++){
      cout << result[i].first << ": " << (result[i].second)/sum << ' ' << endl;
    }
    cout << endl;

    return result;
  }

  void printTableau() {
    cout << "\tc";
    for(int i = 0 ; i < sVars.size() ; i++) {
      cout << '\t' << sVars[i] ;
    }

    for(int i = 0 ; i < xVars.size() ; i++) {
      cout << '\t' << xVars[i];
    }

    cout << endl;

    for(int i = 0 ; i < matrix.size(); i++) {
      for(int j = 0 ; j < matrix[i].size() ; j++) {
        cout << matrix[i][j] << "\t";
      }
      cout << endl;
    }

    cout << endl << endl;

  }

  int rows() {
    return matrix.size();
  }

  int columns() {
    return columns(0);
  }

  int columns(int index) {
    return matrix[index].size();
  }

  double getCoeficient(int row, int column) {
    return matrix[row][column];
  }

  void setCoeficient(int row, int column, double value) {
    matrix[row][column] = value;
  }

  string getVariable(int row) {
    // cout << row << ' ' << matrix[row][0] << endl;
    string result;
    if( (matrix[row][0]) < 0 ) {
      result = "s" + to_string(abs((int)matrix[row][0]));
    
    } else if((matrix[row][0]) > 0) {
      result = "x" + to_string((int)(matrix[row][0]));
    
    } else {
      cout << "Unknwon index. Shouldn't enter here!" << endl;
      exit(1);
    }

    return result;
  }

};

string relatedVar(string v) {
  if(v[0] == 's') {
    v[0] = 'x';
  } else if(v[0] == 'x') {
    v[0] = 's';
  }

  return v;
}

bool lemke_howson (tableau t1, tableau t2, string startPivot, long long maxIterations, long long &iterations) {
  string pivot = startPivot;

  while(true) {
    if(iterations + 1 > maxIterations) {
      return false;
    }

    iterations++;

    // cout << "var_in: " << pivot << endl;

    tableau *cur_tab = nullptr;
    
    if (t1.isMember(pivot)){
      cur_tab = &t1;
      // cout << "cur_tab = t1\n";
    }
    else {
      cur_tab = &t2;
      // cout << "cur_tab = t2\n";
    }

    int col_i = cur_tab->getColumn(pivot);
    int row_i;

    double minimum_ratio = 0x3f3f3f3f;
    for (int i = 0 ; i < cur_tab->rows() ; i++ ){

      if( cur_tab->getCoeficient(i,col_i) < 0 ) {
        double ratio = -cur_tab->getCoeficient(i,1) / cur_tab->getCoeficient(i,col_i);
        // cout << "ratio: " << ratio << endl;
        if( ratio < minimum_ratio ) {
        	// cout << "minimum_ratio "  << ratio << " " << cur_tab->getCoeficient(i,0) << endl;
          minimum_ratio = ratio;
          row_i = i;
        }
      }
    }

    string var_out = cur_tab->getVariable(row_i);
    int col_i_out = cur_tab->getColumn(var_out);

    // cur_tab->printTableau();

    // cout << pivot << " [row_i] = " << row_i << " --- " << stoi(pivot.substr(1,pivot.size()-1)) << endl;
    // cout << "var_out: " << var_out << endl << endl;

    cur_tab->setCoeficient(row_i,col_i_out,-1);
    cur_tab->setCoeficient(row_i,0, 
      (pivot[0] == 'x' ? 1 : -1) * stoi(pivot.substr(1,pivot.size()-1)) );

    for (int j = 1 ; j < cur_tab->columns(row_i) ; j++) {
      if(j != col_i) {
        cur_tab->setCoeficient(row_i,j,  cur_tab->getCoeficient(row_i,j) / -cur_tab->getCoeficient(row_i,col_i));
      }
    }
      
    cur_tab->setCoeficient(row_i,col_i,0);

    for (int i = 0 ; i < cur_tab->rows() ; i++) {
      if( cur_tab->getCoeficient(i,col_i) != 0 ) {

        for (int j = 1 ; j < cur_tab->columns(i) ; j++) {
          cur_tab->setCoeficient(i,j, cur_tab->getCoeficient(i,j) + cur_tab->getCoeficient(i,col_i)*cur_tab->getCoeficient(row_i,j));
        }

        cur_tab->setCoeficient(i,col_i,0);
      }
    }

    // cur_tab->printTableau();

    pivot = relatedVar(var_out);
    if(pivot.substr(1,pivot.size()-1) == startPivot.substr(1,startPivot.size()-1) ) {
      break;
    }
  }

  // cout << "=======\n";
  // t1.printTableau();
  // t1.getEquilibrium();
  // t2.printTableau();
  // t2.getEquilibrium();


  return true;
}

void readInput(vector<vector<int> > &v1, vector<vector<int> > &v2) {
  int rows,columns;
  
  cin >> rows >> columns;

  vector<vector<int> > payoffMatrix(rows, vector<int>(columns*2,0));

  for(int i = 0 ; i < payoffMatrix.size() ; i++ ) {
    for(int j = 0 ; j < payoffMatrix[i].size() ; j++ ) {
      cin >> payoffMatrix[i][j];
    }
  }

  v1.resize(rows,vector<int> (columns, 0) );
  v2.resize(rows,vector<int> (columns, 0) );

  for(int i = 0 ; i < payoffMatrix.size() ; i++) {
    for(int j = 0 ; j < payoffMatrix[i].size() ; j++) {
      if(j%2 == 1) {
        v1[i][j/2] = payoffMatrix[i][j];
      } else {
        v2[i][j/2] = payoffMatrix[i][j];
      }
    }
  }
}

int main() {
  srand(time(0));
  long long maxIterations;
  long long iterations = 0;

  cin >> maxIterations;

  vector<vector<int> > v1,v2;
  
  readInput(v1,v2);
  
  tableau t(v1,true);
  tableau t2(v2,false);

  // t.printTableau();
  // t2.printTableau();


  vector<string> varPool;
  for(int i = 0 ; i < v1.size() + v1[0].size() ; i++) {
    varPool.push_back("x" + to_string(i+1));
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(varPool.begin(),varPool.end(), std::default_random_engine(seed));

  // cout << varPool.size() << endl;

  int index = -1;
  bool foundEquilibrium = false;
  while(++index < varPool.size() && !foundEquilibrium) {
    iterations = 0;
    auto start = std::chrono::system_clock::now();
    
    foundEquilibrium = lemke_howson(t,t2,varPool[index], maxIterations, iterations);
    // foundEquilibrium = lemke_howson(t,t2,"x32", maxIterations, iterations);
    
    auto end = std::chrono::system_clock::now();
    auto elapsed = end - start;
    
    if(foundEquilibrium) {
      cout << "Solved. ";
    } else {
      cout << "Not Solved. ";
    }

    cout << "It took " << iterations << " in " << elapsed.count()/10000000.0 << " seconds using " << varPool[index] << " as first pivot." << endl;
  }

  if(!foundEquilibrium) {
  	iterations = 0;

  	auto start = std::chrono::system_clock::now();
  	foundEquilibrium = lemke_howson(t,t2,varPool[index-1], 0x3f3f3f3f, iterations);
    auto end = std::chrono::system_clock::now();
    
    auto elapsed = end - start;
    
    if(foundEquilibrium) {
      cout << "Solved. ";
    } else {
      cout << "Not Solved. ";
    }

    cout << "It took " << iterations << " in " << elapsed.count()/10000000.0 << " seconds using " << varPool[index-1] << " as first pivot." << endl;
  }
  
  

  return 0;
}
