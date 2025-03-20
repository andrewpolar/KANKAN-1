#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "Urysohn.h"

class Layer {
public:
	Layer(int nUrysohns, int nFunctions, std::vector<double> xmin, std::vector<double> xmax, int nPoints) {
		if (xmin.size() != xmax.size() || xmin.size() != nFunctions) {
			printf("Fatal: sizes of xmin, xmax or nFunctions mismatch\n");
			exit(0);
		}
		for (int i = 0; i < nUrysohns; ++i) {
			_urysohns.push_back(std::make_unique<Urysohn>(xmin, xmax, 0.0, 1.0, nPoints));
		}
	}
	Layer(int nUrysohns, int nFunctions, int nPoints) {
		std::vector<double> xmin;
		std::vector<double> xmax;
		for (int i = 0; i < nFunctions; ++i) {
			xmin.push_back(0.0);
			xmax.push_back(1.0);
		}
		for (int i = 0; i < nUrysohns; ++i) {
			_urysohns.push_back(std::make_unique<Urysohn>(xmin, xmax, 0.0, 1.0, nPoints));
		}
	}
	Layer(const Layer& layer) {
		_urysohns.clear();
		_urysohns = std::vector<std::unique_ptr<Urysohn>>(layer._urysohns.size());
		for (int i = 0; i < layer._urysohns.size(); ++i) {
			_urysohns[i] = std::make_unique<Urysohn>(*layer._urysohns[i]);
		}
	}
	void Input2Output(const std::unique_ptr<double[]>& input, std::unique_ptr<double[]>& output) {
		for (int i = 0; i < _urysohns.size(); ++i) {
			output[i] = _urysohns[i]->GetUrysohn(input);
		}
	}
	void Input2Output(const std::unique_ptr<double[]>& input, std::unique_ptr<double[]>& output, 
		std::unique_ptr<std::unique_ptr<int[]>[]>& indexes) {
		for (int i = 0; i < _urysohns.size(); ++i) {
			output[i] = _urysohns[i]->GetUrysohn(input, indexes[i]);
		}
	}
	void GetDeltas(const std::unique_ptr<double[]>& deltasIn, std::unique_ptr<double[]>& deltasOut,
		const std::unique_ptr<std::unique_ptr<int[]>[]>& indexes) {
		for (int i = 0; i < _urysohns.size(); ++i) {
			_urysohns[i]->UpdateDerivativeVector(deltasIn[i], deltasOut, indexes[i]);
		}
	}
	void Update(const std::unique_ptr<double[]>& input, const std::unique_ptr<double[]>& deltas, double mu) {
		for (int i = 0; i < _urysohns.size(); ++i) {
			_urysohns[i]->Update(deltas[i] * mu, input);
		}
	}
	void IncrementPoins() {
		for (int i = 0; i < _urysohns.size(); ++i) {
			_urysohns[i]->IncrementPoints();
		}
	}
private:
	std::vector<std::unique_ptr<Urysohn>> _urysohns;
};

