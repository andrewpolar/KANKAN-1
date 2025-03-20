#pragma once
#include <memory>
#include "Function.h"
#include "Helper.h"

class Urysohn
{
public:
	Urysohn(std::vector<double> xmin, std::vector<double> xmax, double targetMin, double targetMax, int points_in_function) {
		if (xmin.size() != xmax.size()) {
			printf("Fatal: wrong array sizes\n");
			exit(0);
		}
		int size = (int)xmin.size();
		double oneFunctionMin = targetMin / size;
		double oneFunctionMax = targetMax / size;

		//Sum2IndividualLimits(targetMin, targetMax, size, oneFunctionMin, oneFunctionMax);
		for (int i = 0; i < size; ++i) {
			_functionList.push_back(std::make_unique<Function>(xmin[i], xmax[i], oneFunctionMin, oneFunctionMax, points_in_function));
		}
	}
	Urysohn(const Urysohn& uri) {
		_functionList.clear();
		_functionList = std::vector<std::unique_ptr<Function>>(uri._functionList.size());
		for (int i = 0; i < uri._functionList.size(); ++i) {
			_functionList[i] = std::make_unique<Function>(*uri._functionList[i]);
		}
	}
	double GetUrysohn(const std::unique_ptr<double[]>& inputs, std::unique_ptr<int[]>& indexes) {
		double f = 0.0;
		for (int i = 0; i < _functionList.size(); ++i) {
			f += _functionList[i]->GetFunction(inputs[i], indexes[i]);
		}
		return f;
	}
	double GetUrysohn(const std::unique_ptr<double[]>& inputs) {
		double f = 0.0;
		for (int i = 0; i < _functionList.size(); ++i) {
			f += _functionList[i]->GetFunction(inputs[i]);
		}
		return f;
	}
	void Update(double delta, const std::unique_ptr<double[]>& inputs) {
		for (int i = 0; i < _functionList.size(); ++i) {
			_functionList[i]->Update(inputs[i], delta);
		}
	}
	void UpdateDerivativeVector(const double delta, std::unique_ptr<double[]>& derivatives, const std::unique_ptr<int[]>& indexes) {
		for (int i = 0; i < _functionList.size(); ++i) {
			derivatives[i] += delta * _functionList[i]->GetDerivative(indexes[i]);
		}
	}
	void IncrementPoints() {
		for (int i = 0; i < _functionList.size(); ++i) {
			_functionList[i]->IncrementPoints();
		}
	}
private:
	std::vector<std::unique_ptr<Function>> _functionList;
	void Sum2IndividualLimits(double sumMin, double sumMax, int N, double& xmin, double& xmax) {
		double max_plus_min = (sumMin + sumMax) / N;
		double max_minus_min = (sumMax - sumMin) / (2.0 * 1.96);
		max_minus_min *= max_minus_min;
		max_minus_min *= 12.0;
		max_minus_min /= N;
		max_minus_min = sqrt(max_minus_min);
		xmax = (max_plus_min + max_minus_min) / 2.0;
		xmin = max_plus_min - xmax;
	}
};