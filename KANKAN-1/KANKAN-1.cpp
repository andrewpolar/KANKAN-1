//Concept: Andrew Polar and Mike Poluektov
//Developer Andrew Polar

// License
// If the end user somehow manages to make billions of US dollars using this code,
// and happens to meet the developer begging for change outside a McDonald's,
// he or she is under no obligation to buy the developer a sandwich.

// Symmetry Clause
// Likewise, if the developer becomes rich and famous by publishing this code,
// and meets an unfortunate end user who went bankrupt using it,
// the developer is also under no obligation to buy the end user a sandwich.

//Publications:
//https://www.sciencedirect.com/science/article/abs/pii/S0016003220301149
//https://www.sciencedirect.com/science/article/abs/pii/S0952197620303742
//https://arxiv.org/abs/2305.08194

#include <iostream>
#include "Helper.h"
#include "Function.h"
#include "Urysohn.h"
#include "Layer.h"

//////////// Single function data 
double function(double x) {
	return log2(1.0 + x) + sqrt(abs(x)) + sin(x);
}

std::unique_ptr<double[]> GenerateRandomArray(int N, double min, double max) {
	auto x = std::make_unique<double[]>(N);
	for (int i = 0; i < N; ++i) {
		x[i] = rand() % 1000 / 1000.0;
		x[i] *= (max - min);
		x[i] += min;
	}
	return x;
}

std::unique_ptr<double[]> ComputeSingleFunction(const std::unique_ptr<double[]>& x, int N) {
	auto y = std::make_unique<double[]>(N);
	for (int i = 0; i < N; ++i) {
		y[i] = function(x[i]);
	}
	return y;
}
///////////// End single function data

///////////// Urysohn and Layer
double urysohn(double x1, double x2, double x3, double x4, double x5) {
	return log2(1.0 + x1) + sqrt(abs(x2)) + sin(x3) + cos(x4) + x5;
}

double urysohn2(double x1, double x2, double x3, double x4, double x5) {
	return sin(x1) + sqrt(abs(sin(x2))) + 0.2 * x3 + x4 + log2(1.0 + x5);
}

double urysohn3(double x1, double x2, double x3, double x4, double x5) {
	return sqrt(x1) + cos(x2) + log2(1.0 + x3) + sin(x4) + sqrt(x5 * x5 * x5);
}

std::unique_ptr<std::unique_ptr<double[]>[]> GenerateRandomMatrix(int nRows, int nCols, double min, double max) {
	auto M = std::make_unique<std::unique_ptr<double[]>[]>(nRows);
	for (int i = 0; i < nRows; ++i) {
		M[i] = std::make_unique<double[]>(nCols);
		for (int j = 0; j < nCols; ++j) {
			M[i][j] = rand() % 1000 / 1000.0;
			M[i][j] *= (max - min);
			M[i][j] += min;
		}
	}
	return M;
}

std::unique_ptr<double[]> ComputeUrysohn(const std::unique_ptr<std::unique_ptr<double[]>[]>& M, int nRows) {
	auto v = std::make_unique<double[]>(nRows);
	for (int i = 0; i < nRows; ++i) {
		v[i] = urysohn(M[i][0], M[i][1], M[i][2], M[i][3], M[i][4]);
	}
	return v;
}

std::unique_ptr<std::unique_ptr<double[]>[]> ComputeLayer(const std::unique_ptr<std::unique_ptr<double[]>[]>& M, int nRows) {
	auto v = std::make_unique<std::unique_ptr<double[]>[]>(nRows);
	for (int i = 0; i < nRows; ++i) {
		v[i] = std::make_unique<double[]>(3);
		v[i][0] = urysohn(M[i][0], M[i][1], M[i][2], M[i][3], M[i][4]);
		v[i][1] = urysohn2(M[i][0], M[i][1], M[i][2], M[i][3], M[i][4]);
		v[i][2] = urysohn3(M[i][0], M[i][1], M[i][2], M[i][3], M[i][4]);
	}
	return v;
}
///////////// End Urysohn and Layer

///////////// Determinat dataset
std::unique_ptr<std::unique_ptr<double[]>[]> GenerateInput(int nRecords, int nFeatures, double min, double max) {
	auto x = std::make_unique<std::unique_ptr<double[]>[]>(nRecords);
	for (int i = 0; i < nRecords; ++i) {
		x[i] = std::make_unique<double[]>(nFeatures);
		for (int j = 0; j < nFeatures; ++j) {
			x[i][j] = static_cast<double>((rand() % 10000) / 10000.0);
			x[i][j] *= (max - min);
			x[i][j] += min;
		}
	}
	return x;
}

double determinant(const std::vector<std::vector<double>>& matrix) {
	int n = (int)matrix.size();
	if (n == 1) {
		return matrix[0][0];
	}
	if (n == 2) {
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	}
	double det = 0.0;
	for (int col = 0; col < n; ++col) {
		std::vector<std::vector<double>> subMatrix(n - 1, std::vector<double>(n - 1));
		for (int i = 1; i < n; ++i) {
			int subCol = 0;
			for (int j = 0; j < n; ++j) {
				if (j == col) continue;
				subMatrix[i - 1][subCol++] = matrix[i][j];
			}
		}
		det += (col % 2 == 0 ? 1 : -1) * matrix[0][col] * determinant(subMatrix);
	}
	return det;
}

double ComputeDeterminant(std::unique_ptr<double[]>& input, int N) {
	std::vector<std::vector<double>> matrix(N, std::vector<double>(N, 0.0));
	int cnt = 0;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			matrix[i][j] = input[cnt++];
		}
	}
	return determinant(matrix);
}

std::unique_ptr<double[]> ComputeDeterminantTarget(const std::unique_ptr<std::unique_ptr<double[]>[]>& x, int nMatrixSize, int nRecords) {
	auto target = std::make_unique<double[]>(nRecords);
	int counter = 0;
	while (true) {
		target[counter] = ComputeDeterminant(x[counter], nMatrixSize);
		if (++counter >= nRecords) break;
	}
	return target;
}
///////// End determinant data

///////// Areas of faces of tetrahedron
double Area(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3) {
	double a1 = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	double a2 = (x2 - x1) * (z3 - z1) - (z2 - z1) * (x3 - x1);
	double a3 = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1);
	double A = 0.5 * sqrt(a1 * a1 + a2 * a2 + a3 * a3);
	return A;
}

std::unique_ptr<std::unique_ptr<double[]>[]> MakeRandomMatrix(int rows, int cols, double min, double max) {
	auto matrix = std::make_unique<std::unique_ptr<double[]>[]>(rows);
	for (int i = 0; i < rows; ++i) {
		matrix[i] = std::make_unique<double[]>(cols);
		for (int j = 0; j < cols; ++j) {
			matrix[i][j] = static_cast<double>((rand() % 1000) / 1000.0) * (max - min) + min;
		}
	}
	return matrix;
}

std::unique_ptr<std::unique_ptr<double[]>[]> ComputeTargetMatrix(const std::unique_ptr<std::unique_ptr<double[]>[]>& X, int rows) {
	auto matrix = std::make_unique<std::unique_ptr<double[]>[]>(rows);
	for (int i = 0; i < rows; ++i) {
		matrix[i] = std::make_unique<double[]>(4);
		matrix[i][0] = Area(X[i][0], X[i][1], X[i][2], X[i][3], X[i][4], X[i][5], X[i][6], X[i][7], X[i][8]);
		matrix[i][1] = Area(X[i][0], X[i][1], X[i][2], X[i][3], X[i][4], X[i][5], X[i][9], X[i][10], X[i][11]);
		matrix[i][2] = Area(X[i][0], X[i][1], X[i][2], X[i][6], X[i][7], X[i][8], X[i][9], X[i][10], X[i][11]);
		matrix[i][3] = Area(X[i][3], X[i][4], X[i][5], X[i][6], X[i][7], X[i][8], X[i][9], X[i][10], X[i][11]);
	}
	return matrix;
}
//////////// End tetrahedron

///////// Medians
double Median1(double x1, double y1, double x2, double y2, double x3, double y3) {
	double t1 = x1 - (x2 + x3) / 2.0;
	double t2 = y1 - (y2 + y3) / 2.0;
	t1 *= t1;
	t2 *= t2;
	return sqrt(t1 + t2);
}

double Median2(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double t1 = x2 - (x1 + x3) / 2.0;
	double t2 = y2 - (y1 + y3) / 2.0;
	t1 *= t1;
	t2 *= t2;
	return sqrt(t1 + t2);
}

double Median3(double x1, double y1, double x2, double y2, double x3, double y3)
{
	double t1 = x3 - (x2 + x1) / 2.0;
	double t2 = y3 - (y2 + y1) / 2.0;
	t1 *= t1;
	t2 *= t2;
	return sqrt(t1 + t2);
}

std::unique_ptr<std::unique_ptr<double[]>[]> GenerateInputsMedians(int nRecords, int nFeatures, double min, double max) {
	auto x = std::make_unique<std::unique_ptr<double[]>[]>(nRecords);
	for (int i = 0; i < nRecords; ++i) {
		x[i] = std::make_unique<double[]>(nFeatures);
		for (int j = 0; j < nFeatures; ++j) {
			x[i][j] = static_cast<double>((rand() % 10000) / 10000.0);
			x[i][j] *= (max - min);
			x[i][j] += min;
		}
	}
	return x;
}

std::unique_ptr<std::unique_ptr<double[]>[]> ComputeTargetsMedians(const std::unique_ptr<std::unique_ptr<double[]>[]>& x, int nRecords) {
	auto y = std::make_unique<std::unique_ptr<double[]>[]>(nRecords);
	for (int i = 0; i < nRecords; ++i) {
		y[i] = std::make_unique<double[]>(3);
		for (int j = 0; j < 3; ++j) {
			y[i][0] = Median1(x[i][0], x[i][1], x[i][2], x[i][3], x[i][4], x[i][5]);
			y[i][1] = Median2(x[i][0], x[i][1], x[i][2], x[i][3], x[i][4], x[i][5]);
			y[i][2] = Median3(x[i][0], x[i][1], x[i][2], x[i][3], x[i][4], x[i][5]);
		}
	}
	return y;
}
///////// End medians

///////// Random triangles
std::unique_ptr<std::unique_ptr<double[]>[]> MakeRandomMatrixForTriangles(int rows, int cols, double min, double max) {
	std::unique_ptr<std::unique_ptr<double[]>[]> matrix;
	matrix = std::make_unique<std::unique_ptr<double[]>[]>(rows);
	for (int i = 0; i < rows; ++i) {
		matrix[i] = std::make_unique<double[]>(cols);
		for (int j = 0; j < cols; ++j) {
			matrix[i][j] = static_cast<double>((rand() % 1000) / 1000.0) * (max - min) + min;
		}
	}
	return matrix;
}
double AreaOfTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
	double A = 0.5 * abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	return A;
}
std::unique_ptr<double[]> ComputeAreasOfTriangles(std::unique_ptr<std::unique_ptr<double[]>[]>& matrix, int N) {
	auto u = std::make_unique<double[]>(N);
	for (int i = 0; i < N; ++i) {
		u[i] = AreaOfTriangle(matrix[i][0], matrix[i][1], matrix[i][2], matrix[i][3], matrix[i][4], matrix[i][5]);
	}
	return u;
}
///////// End of random triangles

void TestSingleFunction() {
	int nTrainingRecords = 1000;
	int nValidationRecords = 200;
	double min = 0.0;
	double max = 1.0;
	auto features_training = GenerateRandomArray(nTrainingRecords, min, max);
	auto targets_training = ComputeSingleFunction(features_training, nTrainingRecords);
	auto features_validation = GenerateRandomArray(nValidationRecords, min, max);
	auto targets_validation = ComputeSingleFunction(features_validation, nValidationRecords);

	double argmin = Helper::Min(features_training, nTrainingRecords);
	double argmax = Helper::Max(features_training, nTrainingRecords);
	double targetMin = Helper::Min(targets_training, nTrainingRecords);
	double targetMax = Helper::Max(targets_training, nTrainingRecords);

	auto f = std::make_unique<Function>(argmin, argmax, targetMin, targetMax, 10);

	printf("Training function\n");
	double mu = 0.01;
	for (int epoch = 0; epoch < 20; ++epoch) {
		double error = 0.0;
		for (int i = 0; i < nTrainingRecords; ++i) {
			double model = f->GetFunction(features_training[i]);
			double residual = targets_training[i] - model;
			f->Update(features_training[i], residual * mu);
			error += residual * residual;
		}
		error /= nTrainingRecords;
		error = sqrt(error);
		error /= (targetMax - targetMin);
		printf("%f\n", error);

		if (epoch >= 4 && epoch <= 5) {
			f->IncrementPoints();
		}
	}

	auto copy = std::make_unique<Function>(*f);

	double error = 0.0;
	for (int i = 0; i < nValidationRecords; ++i) {
		double model = copy->GetFunction(features_validation[i]);
		double residual = targets_validation[i] - model;
		error += residual * residual;
	}
	error /= nValidationRecords;
	error = sqrt(error);
	error /= (targetMax - targetMin);
	printf("\nValidation function %f\n\n", error);
}

void TestSingleUrysohn() {
	int nTrainingRecords = 8000;
	int nValidationRecords = 2000;
	int nFeatures = 5;
	double min = 0.0;
	double max = 1.0;

	auto features_training = GenerateRandomMatrix(nTrainingRecords, nFeatures, min, max);
	auto targets_training = ComputeUrysohn(features_training, nTrainingRecords);
	auto features_validation = GenerateRandomMatrix(nValidationRecords, nFeatures, min, max);
	auto targets_validation = ComputeUrysohn(features_validation, nValidationRecords);

	std::vector<double> argmin;
	std::vector<double> argmax;
	double targetMin, targetMax;

	Helper::FindMinMax(argmin, argmax, targetMin, targetMax, features_training, targets_training, nTrainingRecords, nFeatures);

	auto urysohn = std::make_unique<Urysohn>(argmin, argmax, targetMin, targetMax, 12);

	printf("Training urysohn\n");
	double mu = 0.01;
	for (int epoch = 0; epoch < 10; ++epoch) {
		double error = 0.0;
		for (int i = 0; i < nTrainingRecords; ++i) {
			double model = urysohn->GetUrysohn(features_training[i]);
			double residual = targets_training[i] - model;
			error += residual * residual;
			urysohn->Update(residual * mu, features_training[i]);
		}
		error /= nTrainingRecords;
		error = sqrt(error);
		error /= (targetMax - targetMin);
		printf("%f\n", error);

		if (epoch >= 3 && epoch <= 4) {
			urysohn->IncrementPoints();
		}
	}

	auto copy = std::make_unique<Urysohn>(*urysohn);

	double error = 0.0;
	for (int i = 0; i < nValidationRecords; ++i) {
		double model = copy->GetUrysohn(features_validation[i]);
		double residual = targets_validation[i] - model;
		error += residual * residual;
	}
	error /= nValidationRecords;
	error = sqrt(error);
	error /= (targetMax - targetMin);
	printf("\nValidation urysohn %f\n\n", error);
}

void TestLayer() {
	int nTrainingRecords = 8000;
	int nValidationRecords = 2000;
	int nFeatures = 5;
	int nTargets = 3;
	double min = 0.0;
	double max = 1.0;
	double mu = 0.05;

	auto features_training = GenerateRandomMatrix(nTrainingRecords, nFeatures, min, max);
	auto targets_training = ComputeLayer(features_training, nTrainingRecords);
	auto features_validation = GenerateRandomMatrix(nValidationRecords, nFeatures, min, max);
	auto targets_validation = ComputeLayer(features_validation, nValidationRecords);

	std::vector<double> argmin;
	std::vector<double> argmax;
	Helper::FindMinMaxMatrix(argmin, argmax, features_training, nTrainingRecords, nFeatures);

	std::vector<double> tmin;
	std::vector<double> tmax;
	Helper::FindMinMaxMatrix(tmin, tmax, targets_training, nTrainingRecords, nTargets);

	auto layer0 = std::make_unique<Layer>(nTargets, nFeatures, argmin, argmax, 16);

	auto models0 = std::make_unique<double[]>(nTargets);
	auto deltas0 = std::make_unique<double[]>(nTargets);
	auto indexes = std::make_unique<std::unique_ptr<int[]>[]>(nTargets);
	for (int i = 0; i < nTargets; ++i) {
		indexes[i] = std::make_unique<int[]>(nFeatures);
	}

	//training
	printf("Training layer\n");
	for (int epoch = 0; epoch < 10; ++epoch) {
		double error = 0.0;
		for (int i = 0; i < nTrainingRecords; ++i) {
			layer0->Input2Output(features_training[i], models0, indexes);
			for (int j = 0; j < nTargets; ++j) {
				deltas0[j] = targets_training[i][j] - models0[j];
				error += deltas0[j] * deltas0[j];
			}
			layer0->Update(features_training[i], deltas0, mu);
		}
		error /= nTargets;
		error /= nTrainingRecords;
		error = sqrt(error);
		printf("%f\n", error);
		if (2 == epoch) {
			layer0->IncrementPoins();
		}
	}

	auto copy = std::make_unique<Layer>(*layer0);

	//validation
	auto actual_target0 = std::make_unique<double[]>(nValidationRecords);
	auto actual_target1 = std::make_unique<double[]>(nValidationRecords);
	auto actual_target2 = std::make_unique<double[]>(nValidationRecords);
	auto model_target0 = std::make_unique<double[]>(nValidationRecords);
	auto model_target1 = std::make_unique<double[]>(nValidationRecords);
	auto model_target2 = std::make_unique<double[]>(nValidationRecords);
	double error = 0.0;
	for (int i = 0; i < nValidationRecords; ++i) {
		copy->Input2Output(features_validation[i], models0);
		for (int j = 0; j < nTargets; ++j) {
			double err = targets_validation[i][j] - models0[j];
			error += err * err;
		}
		model_target0[i] = models0[0];
		model_target1[i] = models0[1];
		model_target2[i] = models0[2];
		actual_target0[i] = targets_validation[i][0];
		actual_target1[i] = targets_validation[i][1];
		actual_target2[i] = targets_validation[i][2];
	}
	error /= nTargets;
	error /= nValidationRecords;
	error = sqrt(error);
	printf("\nValidation layer %f\n", error);

	double p1 = Helper::Pearson(model_target0, actual_target0, nValidationRecords);
	double p2 = Helper::Pearson(model_target1, actual_target1, nValidationRecords);
	double p3 = Helper::Pearson(model_target2, actual_target2, nValidationRecords);
	printf("Pearsons: %f %f %f\n\n", p1, p2, p3);
}

void Det_4_4() {
	int nTrainingRecords = 100000;
	int nValidationRecords = 20000;
	int nMatrixSize = 4;
	int nFeatures = nMatrixSize * nMatrixSize;
	int nTargets = 1;
	double min = 0.0;
	double max = 1.0;
	auto features_training = GenerateInput(nTrainingRecords, nFeatures, min, max);
	auto features_validation = GenerateInput(nValidationRecords, nFeatures, min, max);
	auto targets_training = ComputeDeterminantTarget(features_training, nMatrixSize, nTrainingRecords);
	auto targets_validation = ComputeDeterminantTarget(features_validation, nMatrixSize, nValidationRecords);

	clock_t start_application = clock();
	clock_t current_time = clock();

	//find limits
	std::vector<double> argmin;
	std::vector<double> argmax;
	double targetMin;
	double targetMax;
	Helper::FindMinMax(argmin, argmax, targetMin, targetMax, features_training, targets_training,
		nTrainingRecords, nFeatures);

	//normalize targets, it is not necessary, but sometimes converges faster
	for (int i = 0; i < nTrainingRecords; ++i) {
		targets_training[i] = (targets_training[i] - targetMin) / (targetMax - targetMin);
	}
	for (int i = 0; i < nValidationRecords; ++i) {
		targets_validation[i] = (targets_validation[i] - targetMin) / (targetMax - targetMin);
	}

	//configuration
	int nU0 = 50;
	int nU1 = 1;

	//instantiation of layers
	auto layer0 = std::make_unique<Layer>(nU0, nFeatures, argmin, argmax, 3);
	auto layer1 = std::make_unique<Layer>(nU1, nU0, 30);

	//auxiliary data buffers for a quick moving data between methods
	auto models0 = std::make_unique<double[]>(nU0);
	auto models1 = std::make_unique<double[]>(nU1);

	auto deltas1 = std::make_unique<double[]>(nU1);
	auto deltas0 = std::make_unique<double[]>(nU0);

	auto indexes0 = std::make_unique<std::unique_ptr<int[]>[]>(nU0);
	for (int i = 0; i < nU0; ++i) {
		indexes0[i] = std::make_unique<int[]>(nFeatures);
	}

	auto indexes1 = std::make_unique<std::unique_ptr<int[]>[]>(nU1);
	for (int i = 0; i < nU1; ++i) {
		indexes1[i] = std::make_unique<int[]>(nU0);
	}

	auto actual_validation = std::make_unique<double[]>(nValidationRecords);

	//training
	printf("Training determinants of random 4 * 4 matrices\n");
	for (int epoch = 0; epoch < 50; ++epoch) {
		for (int i = 0; i < nTrainingRecords; ++i) {
			//forward feeding by two layers
			layer0->Input2Output(features_training[i], models0, indexes0);
			layer1->Input2Output(models0, models1, indexes1);

			//computing residual error
			for (int j = 0; j < nTargets; ++j) {
				deltas1[j] = (targets_training[i] - models1[j]);
			}

			//back propagation
			std::fill(deltas0.get(), deltas0.get() + nU0, 0.0);
			layer1->GetDeltas(deltas1, deltas0, indexes1);

			//updating of two layers
			layer1->Update(models0, deltas1, 0.005);
			layer0->Update(features_training[i], deltas0, 1.0);
		}

		//validation at the end of each epoch
		double error = 0.0;
		for (int i = 0; i < nValidationRecords; ++i) {
			layer0->Input2Output(features_validation[i], models0);
			layer1->Input2Output(models0, models1);
			actual_validation[i] = models1[0];
			error += (targets_validation[i] - models1[0]) * (targets_validation[i] - models1[0]);
		}
		double pearson = Helper::Pearson(targets_validation, actual_validation, nValidationRecords);
		error /= nValidationRecords;
		error = sqrt(error);
		current_time = clock();
		printf("Epoch %d, current relative error %f, pearson %f, time %2.3f\n", epoch, error, pearson, (double)(current_time - start_application) / CLOCKS_PER_SEC);
	}
	printf("\n");
}

void Tetrahedron() {
	const int nTrainingRecords = 500000;
	const int nValidationRecords = 50000;
	const int nFeatures = 12;
	const int nTargets = 4;
	const double min = 0.0;
	const double max = 1.0;
	auto features_training = MakeRandomMatrix(nTrainingRecords, nFeatures, min, max);
	auto features_validation = MakeRandomMatrix(nValidationRecords, nFeatures, min, max);
	auto targets_training = ComputeTargetMatrix(features_training, nTrainingRecords);
	auto targets_validation = ComputeTargetMatrix(features_validation, nValidationRecords);

	//data is ready, we start training
	clock_t start_application = clock();
	clock_t current_time = clock();

	std::vector<double> argmin;
	std::vector<double> argmax;
	Helper::FindMinMaxMatrix(argmin, argmax, features_training, nTrainingRecords, nFeatures);

	int nU0 = 50;
	int nU1 = 10;
	int nU2 = nTargets;

	auto layer0 = std::make_unique<Layer>(nU0, nFeatures, argmin, argmax, 6);
	auto layer1 = std::make_unique<Layer>(nU1, nU0, 12);
	auto layer2 = std::make_unique<Layer>(nU2, nU1, 22);

	auto models0 = std::make_unique<double[]>(nU0);
	auto models1 = std::make_unique<double[]>(nU1);
	auto models2 = std::make_unique<double[]>(nU2);

	auto deltas2 = std::make_unique<double[]>(nU2);
	auto deltas1 = std::make_unique<double[]>(nU1);
	auto deltas0 = std::make_unique<double[]>(nU0);

	auto indexes0 = std::make_unique<std::unique_ptr<int[]>[]>(nU0);
	for (int i = 0; i < nU0; ++i) {
		indexes0[i] = std::make_unique<int[]>(nFeatures);
	}

	auto indexes1 = std::make_unique<std::unique_ptr<int[]>[]>(nU1);
	for (int i = 0; i < nU1; ++i) {
		indexes1[i] = std::make_unique<int[]>(nU0);
	}

	auto indexes2 = std::make_unique<std::unique_ptr<int[]>[]>(nU2);
	for (int i = 0; i < nU2; ++i) {
		indexes2[i] = std::make_unique<int[]>(nU1);
	}

	auto actual0 = std::make_unique<double[]>(nValidationRecords);
	auto actual1 = std::make_unique<double[]>(nValidationRecords);
	auto actual2 = std::make_unique<double[]>(nValidationRecords);
	auto actual3 = std::make_unique<double[]>(nValidationRecords);

	auto computed0 = std::make_unique<double[]>(nValidationRecords);
	auto computed1 = std::make_unique<double[]>(nValidationRecords);
	auto computed2 = std::make_unique<double[]>(nValidationRecords);
	auto computed3 = std::make_unique<double[]>(nValidationRecords);

	printf("Training areas of faces of random tetrahedrons\n");
	for (int epoch = 0; epoch < 16; ++epoch) {
		for (int i = 0; i < nTrainingRecords; ++i) {
			layer0->Input2Output(features_training[i], models0, indexes0);
			layer1->Input2Output(models0, models1, indexes1);
			layer2->Input2Output(models1, models2, indexes2);

			for (int j = 0; j < nTargets; ++j) {
				deltas2[j] = targets_training[i][j] - models2[j];
			}

			std::fill(deltas0.get(), deltas0.get() + nU0, 0.0);
			std::fill(deltas1.get(), deltas1.get() + nU1, 0.0);

			layer2->GetDeltas(deltas2, deltas1, indexes2);
			layer1->GetDeltas(deltas1, deltas0, indexes1);

			layer2->Update(models1, deltas2, 0.005);
			layer1->Update(models0, deltas1, 0.1);
			layer0->Update(features_training[i], deltas0, 0.1);
		}

		double error = 0.0;
		for (int i = 0; i < nValidationRecords; ++i) {
			layer0->Input2Output(features_validation[i], models0);
			layer1->Input2Output(models0, models1);
			layer2->Input2Output(models1, models2);

			for (int j = 0; j < nTargets; ++j) {
				double err = targets_validation[i][j] - models2[j];
				error += err * err;
			}

			actual0[i] = targets_validation[i][0];
			actual1[i] = targets_validation[i][1];
			actual2[i] = targets_validation[i][2];
			actual3[i] = targets_validation[i][3];

			computed0[i] = models2[0];
			computed1[i] = models2[1];
			computed2[i] = models2[2];
			computed3[i] = models2[3];
		}
		double p1 = Helper::Pearson(computed0, actual0, nValidationRecords);
		double p2 = Helper::Pearson(computed1, actual1, nValidationRecords);
		double p3 = Helper::Pearson(computed2, actual2, nValidationRecords);
		double p4 = Helper::Pearson(computed3, actual3, nValidationRecords);

		error /= nTargets;
		error /= nValidationRecords;
		error = sqrt(error);
		current_time = clock();
		printf("Epoch %d, RMSE %f, Pearsons: %f %f %f %f, time %2.3f\n", epoch, error, p1, p2, p3, p4,
			(double)(current_time - start_application) / CLOCKS_PER_SEC);
	}
	printf("\n");
}

void Medians() {
	int nTrainingRecords = 10000;
	int nValidationRecords = 2000;
	int nFeatures = 6;
	int nTargets = 3;
	double min = 0.0;
	double max = 1.0;
	auto features_training = GenerateInputsMedians(nTrainingRecords, nFeatures, min, max);
	auto features_validation = GenerateInputsMedians(nValidationRecords, nFeatures, min, max);
	auto targets_training = ComputeTargetsMedians(features_training, nTrainingRecords);
	auto targets_validation = ComputeTargetsMedians(features_validation, nValidationRecords);

	//data is ready, we start training
	clock_t start_application = clock();
	clock_t current_time = clock();

	std::vector<double> argmin;
	std::vector<double> argmax;
	Helper::FindMinMaxMatrix(argmin, argmax, features_training, nTrainingRecords, nFeatures);

	int nU0 = 20;
	int nU1 = 10;
	int nU2 = nTargets;

	auto layer0 = std::make_unique<Layer>(nU0, nFeatures, argmin, argmax, 6);
	auto layer1 = std::make_unique<Layer>(nU1, nU0, 12);
	auto layer2 = std::make_unique<Layer>(nU2, nU1, 22);

	auto models0 = std::make_unique<double[]>(nU0);
	auto models1 = std::make_unique<double[]>(nU1);
	auto models2 = std::make_unique<double[]>(nU2);

	auto deltas2 = std::make_unique<double[]>(nU2);
	auto deltas1 = std::make_unique<double[]>(nU1);
	auto deltas0 = std::make_unique<double[]>(nU0);

	auto indexes0 = std::make_unique<std::unique_ptr<int[]>[]>(nU0);
	for (int i = 0; i < nU0; ++i) {
		indexes0[i] = std::make_unique<int[]>(nFeatures);
	}

	auto indexes1 = std::make_unique<std::unique_ptr<int[]>[]>(nU1);
	for (int i = 0; i < nU1; ++i) {
		indexes1[i] = std::make_unique<int[]>(nU0);
	}

	auto indexes2 = std::make_unique<std::unique_ptr<int[]>[]>(nU2);
	for (int i = 0; i < nU2; ++i) {
		indexes2[i] = std::make_unique<int[]>(nU1);
	}

	auto actual0 = std::make_unique<double[]>(nValidationRecords);
	auto actual1 = std::make_unique<double[]>(nValidationRecords);
	auto actual2 = std::make_unique<double[]>(nValidationRecords);

	auto computed0 = std::make_unique<double[]>(nValidationRecords);
	auto computed1 = std::make_unique<double[]>(nValidationRecords);
	auto computed2 = std::make_unique<double[]>(nValidationRecords);

	printf("Training medians of random triangles\n");
	for (int epoch = 0; epoch < 30; ++epoch) {
		for (int i = 0; i < nTrainingRecords; ++i) {
			layer0->Input2Output(features_training[i], models0, indexes0);
			layer1->Input2Output(models0, models1, indexes1);
			layer2->Input2Output(models1, models2, indexes2);

			for (int j = 0; j < nTargets; ++j) {
				deltas2[j] = targets_training[i][j] - models2[j];
			}

			std::fill(deltas0.get(), deltas0.get() + nU0, 0.0);
			std::fill(deltas1.get(), deltas1.get() + nU1, 0.0);

			layer2->GetDeltas(deltas2, deltas1, indexes2);
			layer1->GetDeltas(deltas1, deltas0, indexes1);

			layer2->Update(models1, deltas2, 0.005);
			layer1->Update(models0, deltas1, 0.1);
			layer0->Update(features_training[i], deltas0, 0.1);
		}

		double error = 0.0;
		for (int i = 0; i < nValidationRecords; ++i) {
			layer0->Input2Output(features_validation[i], models0);
			layer1->Input2Output(models0, models1);
			layer2->Input2Output(models1, models2);

			for (int j = 0; j < nTargets; ++j) {
				double err = targets_validation[i][j] - models2[j];
				error += err * err;
			}

			actual0[i] = targets_validation[i][0];
			actual1[i] = targets_validation[i][1];
			actual2[i] = targets_validation[i][2];

			computed0[i] = models2[0];
			computed1[i] = models2[1];
			computed2[i] = models2[2];
		}
		double p1 = Helper::Pearson(computed0, actual0, nValidationRecords);
		double p2 = Helper::Pearson(computed1, actual1, nValidationRecords);
		double p3 = Helper::Pearson(computed2, actual2, nValidationRecords);

		error /= nTargets;
		error /= nValidationRecords;
		error = sqrt(error);
		current_time = clock();
		printf("Epoch %d, RMSE %f, Pearsons: %f %f %f, time %2.3f\n", epoch, error, p1, p2, p3,
			(double)(current_time - start_application) / CLOCKS_PER_SEC);
	}
	printf("\n");
}

void AreasOfTriangles() {
	int nFeatures = 6;
	int nTargets = 1;
	int nTrainingRecords = 10000;
	int nValidationRecords = 2000;
	auto features_training = MakeRandomMatrixForTriangles(nTrainingRecords, nFeatures, 0.0, 1.0);
	auto features_validation = MakeRandomMatrixForTriangles(nValidationRecords, nFeatures, 0.0, 1.0);
	auto targets_training = ComputeAreasOfTriangles(features_training, nTrainingRecords);
	auto targets_validation = ComputeAreasOfTriangles(features_validation, nValidationRecords);

	//data is ready, we start training
	clock_t start_application = clock();
	clock_t current_time = clock();

	std::vector<double> argmin;
	std::vector<double> argmax;
	Helper::FindMinMaxMatrix(argmin, argmax, features_training, nTrainingRecords, nFeatures);

	int nU0 = 40;
	int nU1 = 6;
	int nU2 = nTargets;

	auto layer0 = std::make_unique<Layer>(nU0, nFeatures, argmin, argmax, 3);
	auto layer1 = std::make_unique<Layer>(nU1, nU0, 12);
	auto layer2 = std::make_unique<Layer>(nU2, nU1, 12);

	auto models0 = std::make_unique<double[]>(nU0);
	auto models1 = std::make_unique<double[]>(nU1);
	auto models2 = std::make_unique<double[]>(nU2);

	auto deltas2 = std::make_unique<double[]>(nU2);
	auto deltas1 = std::make_unique<double[]>(nU1);
	auto deltas0 = std::make_unique<double[]>(nU0);

	auto indexes0 = std::make_unique<std::unique_ptr<int[]>[]>(nU0);
	for (int i = 0; i < nU0; ++i) {
		indexes0[i] = std::make_unique<int[]>(nFeatures);
	}

	auto indexes1 = std::make_unique<std::unique_ptr<int[]>[]>(nU1);
	for (int i = 0; i < nU1; ++i) {
		indexes1[i] = std::make_unique<int[]>(nU0);
	}

	auto indexes2 = std::make_unique<std::unique_ptr<int[]>[]>(nU2);
	for (int i = 0; i < nU2; ++i) {
		indexes2[i] = std::make_unique<int[]>(nU1);
	}

	auto actual0 = std::make_unique<double[]>(nValidationRecords);
	auto computed0 = std::make_unique<double[]>(nValidationRecords);

	printf("Training for areas of random triangles\n");
	for (int epoch = 0; epoch < 30; ++epoch) {
		for (int i = 0; i < nTrainingRecords; ++i) {
			layer0->Input2Output(features_training[i], models0, indexes0);
			layer1->Input2Output(models0, models1, indexes1);
			layer2->Input2Output(models1, models2, indexes2);

			for (int j = 0; j < nTargets; ++j) {
				deltas2[j] = targets_training[i] - models2[j];
			}

			std::fill(deltas0.get(), deltas0.get() + nU0, 0.0);
			std::fill(deltas1.get(), deltas1.get() + nU1, 0.0);

			layer2->GetDeltas(deltas2, deltas1, indexes2);
			layer1->GetDeltas(deltas1, deltas0, indexes1);

			layer2->Update(models1, deltas2, 0.01);
			layer1->Update(models0, deltas1, 0.01);
			layer0->Update(features_training[i], deltas0, 1.0);
		}

		double error = 0.0;
		for (int i = 0; i < nValidationRecords; ++i) {
			layer0->Input2Output(features_validation[i], models0);
			layer1->Input2Output(models0, models1);
			layer2->Input2Output(models1, models2);

			for (int j = 0; j < nTargets; ++j) {
				double err = targets_validation[i] - models2[j];
				error += err * err;
			}

			actual0[i] = targets_validation[i];
			computed0[i] = models2[0];
		}
		double p1 = Helper::Pearson(computed0, actual0, nValidationRecords);

		error /= nTargets;
		error /= nValidationRecords;
		error = sqrt(error);
		current_time = clock();
		printf("Epoch %d, RMSE %f, Pearson: %f, time %2.3f\n", epoch, error, p1,
			(double)(current_time - start_application) / CLOCKS_PER_SEC);
	}
	printf("\n");
}

int main() {
	srand((unsigned int)time(NULL));

	//This is stable reusable code. KAN has layers, layers have urysohns, each urysohn is sum of functions.
	//Here I show the entire training methods: functions, urysohns, layers and KAN use same training concept,
	//which is call Newton-Kaczmarz method, published in 2021.
	TestSingleFunction();
	TestSingleUrysohn();
	TestLayer();

	//Related targets, the medians of random triangles.
	Medians();

	//Areas of random triangles.
	AreasOfTriangles();

	//This simple unit test, features are random matrices of 4 by 4, targets are their determinants.
	Det_4_4();

	//Related targets, the areas of the faces of tetrahedron given by random vertices.
	Tetrahedron();
}

