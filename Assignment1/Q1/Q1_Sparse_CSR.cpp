#include <bits/stdc++.h>

using namespace std;

typedef struct MATRIX_CSR_FORMAT {
	vector<int> row_indices;
	vector<int> col_indices;
	vector<int> val;

	void populate(int dense_B[][1000], int m, int n) {
		int row_end = 0;
		this->row_indices.push_back(row_end);
		for(int i = 1 ; i <= m ; i++) {
			for(int j = 1 ; j <= n ; j++) {
				if(!dense_B[i][j]) continue;
				this->col_indices.push_back(j);
				this->val.push_back(dense_B[i][j]);
				row_end++;
			}
			this->row_indices.push_back(row_end);
		}
	}

	int get_element(int i, int j) {
		int search_start = this->row_indices[i - 1];
		int search_end = this->row_indices[i];
		for(int k = search_start ; k < search_end ; k++ ) {
			if(this->col_indices[k] == j) 
				return this->val[k];
		}
		return 0;
	}

	void print() {
		cout << "ROW_INDICES : ";
		for(int i = 0 ; i < this->row_indices.size() ; i++){ cout << this->row_indices[i] << " "; }
		cout << endl;
		cout << "COL_INDICES : ";
		for(int i = 0 ; i < this->col_indices.size() ; i++){ cout << this->col_indices[i] << " "; }
		cout << endl;
		cout << "VALUES : ";
		for(int i = 0 ; i < this->val.size() ; i++){ cout << this->val[i] << " "; }
		cout << endl;
	}

}MAT_CSR;

void print_dense(int dense_B[][1000], int m, int n) {
	for(int i = 1 ; i <= m ; i++) {
		for(int j = 1 ; j <= n ; j++) {
			cout << dense_B[i][j] << " ";
		}
		cout << endl;
	}
}

MAT_CSR make_csr(int dense_B[][1000], int m, int n) {
	MAT_CSR B;
	B.populate(dense_B, m, n);
	return B;
}

MAT_CSR mat_mul_csr(MAT_CSR A, MAT_CSR B) {
	int dense_c[100][1000];

	int p = A.row_indices.size() - 1;
	int q = *max_element(A.col_indices.begin(), A.col_indices.end());
	int q_temp = B.row_indices.size() - 1;
	if(q != q_temp) {
		cout << "Matrix input dimensions are not of the right size\n Returning first matrix";
		return A;
	}
	int r = *max_element(B.col_indices.begin(), B.col_indices.end());

	for(int i = 1 ; i <= p ; i++) {
		for(int j = 1; j <= r; j++) {
			dense_c[i][j] = 0;
		}
	}


	for(int k = 1 ; k <= r ; k++) {
		for(int i = 1 ; i <= p ; i++ ) {
			for(int j = A.row_indices[i - 1] ; j < A.row_indices[i] ; j++) {
				dense_c[i][k] += A.val[j] * B.get_element(A.col_indices[j], k);
			}
		}
	}
	return make_csr(dense_c, p, r);
}

int main() {
	int m, n;
	int dense_A[100][1000], dense_B[100][1000];

	// input first matrix
	cin >> m >> n;
	for(int i = 1 ; i <= m ; i++) {
		for(int j = 1 ; j <= n ; j++) {
			cin >> dense_A[i][j];
		}
	}
 	MAT_CSR A = make_csr(dense_A, m, n);

 	// input second matrix
 	cin >> m >> n;
	for(int i = 1 ; i <= m ; i++) {
		for(int j = 1 ; j <= n ; j++) {
			cin >> dense_B[i][j];
		}
	}
 	MAT_CSR B = make_csr(dense_B, m, n);
 	MAT_CSR C = mat_mul_csr(A, B);

 	C.print();
}