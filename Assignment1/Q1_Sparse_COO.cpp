#include <bits/stdc++.h>

using namespace std;

typedef struct matrix_element {
	int row;
	int col;
	int val;
}mat_el;

bool get_rows(mat_el a, mat_el b) {
	return a.row < b.row;
}

bool get_cols(mat_el a, mat_el b) {
	return a.col < b.col;
}

typedef struct sp_mat {
	vector<mat_el> list;

	// getter for element at (i, j)
	int get_element(int i, int j) {
		mat_el tmp;
		for(int k = 0 ; k < list.size() ; k++) {
			tmp = this->list[k];
			if(tmp.row == i && tmp.col == j)
				return tmp.val;
		}
		return 0;
	}

	// setter for element at (i, j)
	void set_element(int i, int j, int val) {
		int k;
		mat_el tmp;
		for(k = 0 ; k < this->list.size() ; k++) {
			tmp = this->list[k];
			if(tmp.row == i && tmp.col == j)
				break;
		}
		if(k == list.size()){
			tmp.row = i;
			tmp.col = j;
			tmp.val = val;
			this->list.push_back(tmp);
		}
		else {
			this->list[k].val = val;
		}
	}

	// no of rows
	int m() {
		return max_element(this->list.begin(), this->list.end(), get_rows)->row;
	}

	// no of columns
	int n() {
		return max_element(this->list.begin(), this->list.end(), get_cols)->col;
	}

	void print() {
		for(int i = 0 ; i < this->list.size() ; i++ ) {
			printf("%d %d %d\n", this->list[i].row, this->list[i].col, this->list[i].val); 
		}
		cout << "----" << endl; 
	}

}sp_mat;

sp_mat sparse_mat_mul(sp_mat A, sp_mat B) {

	sp_mat C;
	int no_of_rows_B = B.n();
	cout << no_of_rows_B << endl;
	int add_term;
	for(int j = 1 ; j <= no_of_rows_B ; j++) {
		for(int i = 0 ; i < A.list.size() ; i++) {
			// cout << C.get_element(i, j) << " " <<  A.list[i].val << " " << B.get_element(A.list[i].col, j);
			add_term = C.get_element(A.list[i].row, j) + A.list[i].val * B.get_element(A.list[i].col, j);
			C.set_element(A.list[i].row, j, add_term);
		}
	}
	return C;
}

int main() {
	int nz, row, col, val;
	sp_mat A, B;
	mat_el tmp;
	cin >> nz;
	for(int i = 0 ; i < nz ; i++) {
		cin >> row >> col >> val;
		A.set_element(row, col, val);
	}

	cin >> nz;
	for(int i = 0 ; i < nz ; i++) {
		cin >> row >> col >> val;
		B.set_element(row, col, val);
	}
	A.print();
	B.print();
	sp_mat C = sparse_mat_mul(A, B);
	C.print();
}