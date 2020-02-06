#include <bits/stdc++.h>
#include <math.h>
#define ld long double
#define ll long long int

using namespace std;


typedef struct matrix {
	ld val[100][100];
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
		return k < this->m;
	}

	bool in_column(int k) {
		return k < this->n;
	}

}mat;

mat mat_copy(mat A) {
	mat B = matrix(A.m, A.n);
	for(int i = 1 ; i <= A.m ; i++) {
		for(int j = 1 ; j <= A.n ; j++) 
			B.val[i][j] = A.val[i][j];
	}
	return B;
}

mat mat_mult(mat A, mat B) {
	mat C = matrix(A.m, B.n);
	for(int i = 1 ; i <= A.m ; i++) {
		for(int j = 1 ; j <= B.n ; j++) {
			for(int k = 1 ; k <= B.m ; k++) 
				C.val[i][j] += A.val[i][k] * B.val[k][j];
		}
	} 
	return C;
}

float mat_l2(mat A, mat B) {
	float l2 = 0;
	for(int i = 1 ; i <= A.m ; i++) {
		for(int j = 1 ; j <= A.n ; j++ )
			l2 += pow(A.val[i][j] - B.val[i][j], 2);
	}
	return l2;
}

void mylu_decompose(mat* A) {
	for(int k = 1 ; k < A->n ; k++ ) {
		for(int p = k + 1 ; p <= A->n ; p++) {
			A->val[p][k] /= A->val[k][k];
		}

		for(int i = k + 1 ; i <= A->n ; i++) {
			for(int j = k + 1 ; j <= A->n ; j++)
				A->val[i][j] = A->val[i][j] - A->val[i][k] * A->val[k][j];
			
		}
	}	
}
/* HESSENBERG MATRIX */

void mylu_decompose_hessenberg(mat* A) {
	for(int k = 1 ; k < A->n ; k++ ) {
		A->val[k + 1][k] /= A->val[k][k];

		for(int i = k + 1 ; i <= A->n ; i++) {
			for(int j = k + 1 ; j <= A->n ; j++)
				A->val[i][j] = A->val[i][j] - A->val[i][k] * A->val[k][j];
			
		}
	}	
}

/* HESSENBERG MATRIX */

mat forward_sub(mat A, mat b) {
	mat x = matrix(A.m, b.n); // b.n == 1
	float sum;
	for(int i = 1 ; i <= A.n; i++) {
		sum = 0;
		for(int j = 1 ; j < i ; j++) 
			sum += A.val[i][j] * x.val[j][1];
		x.val[i][1] = (b.val[i][1] - sum) / 1; // L[i][i] == 1
	}
	return x;
}

// The upper traingular matrix is stored in A
mat backward_sub(mat A, mat b) {
	mat x = matrix(A.m, b.n);
	float sum;
	for(int i = A.n ; i >= 1 ; i--) {
		sum = 0;
		for(int k = i + 1 ; k <= A.n ; k++) 
			sum += A.val[i][k] * x.val[k][1];
		x.val[i][1] = (b.val[i][1] - sum) / A.val[i][i];
	}
	return x;
}

mat lu_solve(mat* A, mat b, int is_hessenberg) {
	if(is_hessenberg)
		mylu_decompose_hessenberg(A);
	else
		mylu_decompose(A);
	mat t = forward_sub(*A, b);
	mat x = backward_sub(*A, t);
	return x;
}


int main() {

	int n, is_hessenberg = 0;
	cin >> n;

	mat A = matrix(n, n);
	for(int i = 1 ; i <= n ; i++) {
		for(int j = 1 ; j <= n ; j++) {
			cin >> A.val[i][j];
		}
	}
	mat b = matrix(n, 1);
	for(int i = 1 ; i <= n; i++)
		cin >> b.val[i][1];
	cin >> is_hessenberg;

	// Copying matrix A for further use
	mat A_copy = mat_copy(A);

	// Solve
	mat x = lu_solve(&A, b, is_hessenberg);
	cout << "x is : " << endl;
	x.print();
	mat C = mat_mult(A_copy, x);
	float result = mat_l2(b, C);

	// Compute ||b - Ax||2 to find accuracy of obtained x vector
	cout << "L2 Norm of b - Ax = " << result << endl;
	return 0;
}