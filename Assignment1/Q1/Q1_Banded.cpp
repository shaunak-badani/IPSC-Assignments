#include <bits/stdc++.h>
#define ll long long int

using namespace std;

typedef struct matrix {
	ll val[100][100];
	ll m;
	ll n;

	// constructor
	matrix(ll m, ll n) {
		for(int i = 1 ; i <= m ; i++) {
			for(int j = 1 ; j <= n ; j++) {
				this->val[i][j] = 0;
			}
		}
		this->m = m;
		this->n = n;
	}

	void print() {
		for(int i = 1 ; i <= this->m ; i++) {
			for(int j = 1 ; j <= this->n ; j++) {
				cout << this->val[i][j] << " ";
			}
			cout << endl;
		}
		cout << "---" << endl;
	}

	bool in_row(int k) {
		return k <= this->m;
	}

	bool in_column(int k) {
		return k <= this->n;
	}

}mat;


// Function to find upper and lower bandwidth of the matrix
pair<int, int> bandwidth(mat A) {

	int upper_limit_search = min(A.m, A.n);
	bool bandwith_found = false;

	// lower bandwidth
	ll lower_bandwidth = A.m;
	for(int j = 1 ; j <= A.m ; j++) {
		for(int i = 1 ; i <= j && A.in_column(i) ; i++  ) {
			if(A.val[lower_bandwidth - 1 + i ][i] != 0){ bandwith_found = true; break; }
		}
		if(bandwith_found)
			break;
		lower_bandwidth = A.m - j;
	}

	// upper bandwidth
	bandwith_found = false;
	ll upper_bandwidth = A.n;
	for(int j = 1 ; j <= A.n ; j++) {
		for(int i = 1 ; i <= j && A.in_row(i) ; i++  ) {
			if(A.val[i][upper_bandwidth - 1 + i] != 0){ bandwith_found = true; break; }
		}
		// cout << endl;
		if(bandwith_found)
			break;
		upper_bandwidth = A.n - j;
	}
	if(lower_bandwidth !=0 ) lower_bandwidth -= 1;
	if(upper_bandwidth !=0 ) upper_bandwidth -= 1;
	return make_pair(lower_bandwidth, upper_bandwidth );
}

// Function to find the banded format storage
// Takes two arguments, the matrix, and the upper and lower bandwith as a pair of integers
mat make_band_matrix(mat A, pair<int, int> bandwidths) {
	int lb = bandwidths.first;
	int ub = bandwidths.second;

	mat A_band = matrix(lb + ub + 1, A.n);
	for(int i = 1 ; i <= A.m ; i++) {
		for(int j = 1 ; j <= A.n ; j++) 
			A_band.val[i - j + 1 + ub][j] = A.val[i][j];
	}
	return A_band;
}

mat multiply_band_matrices(mat B, mat C) {
	pair<int, int> band_b = bandwidth(B);
	pair<int, int> band_c = bandwidth(C);

	mat B_band = make_band_matrix(B, band_b);
	mat C_band = make_band_matrix(C, band_c);

	mat A = matrix(B.m, C.n);
	for(int i = 1 ; i <= B.m ; i++) {
		for(int j = 1 ; j <= C.n ; j++) {
			for(int k = 1 ; k <= C.m ; k++)
				A.val[i][j] += B_band.val[i - j + 1 + band_b.second][k] * C_band.val[k - j + 1 + band_c.second][j]; 
		}
	}
	return A;
}


int main() {
	int m, n, inp;
	cin >> m >> n;
	mat B = matrix(m, n);

	for(ll i = 1 ; i <= m ; i++) {
		for(ll j = 1 ; j <= n ; j++) {
			cin >> inp;
			B.val[i][j] = inp;
		}
	}

	cin >> m >> n;
	mat C = matrix(m, n);

	for(ll i = 1 ; i <= m ; i++) {
		for(ll j = 1 ; j <= n ; j++) {
			cin >> inp;
			C.val[i][j] = inp;
		}
	}
	mat B_band = make_band_matrix(B, bandwidth(B));
	mat A = multiply_band_matrices(B, C);
	A.print();
}
