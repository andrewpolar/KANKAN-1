#pragma once
#include <memory>
#include <vector>

class Function
{
public:
    Function(double xmin, double xmax, double ymin, double ymax, int points) {
        _xmin = xmin;
        _xmax = xmax;
        SetRandomFunction(ymin, ymax, points);
        SetLimits();
    }
    Function(const Function& uni) {
        _xmin = uni._xmin;
        _xmax = uni._xmax;
        _deltax = uni._deltax;
        _y.clear();
        for (int i = 0; i < uni._y.size(); i++) {
            _y.push_back(uni._y[i]);
        }
    }
    void Update(double x, double residual) {
        FitDefinition(x);
        double R = (x - _xmin) / _deltax;
        int index = (int)(R);
        double offset = R - index;
        double tmp = residual * offset;
        _y[index + 1] += tmp;
        _y[index] += residual - tmp;
    }
    double GetDerivative(int index) {
        return (_y[index + 1] - _y[index]) / _deltax;
    }
    double GetFunction(double x, int& index) {
        if (x <= _xmin) {
            index = 0;
            return _y[0];
        }
        if (x >= _xmax) {
            index = (int)_y.size() - 2;
            return _y[_y.size() - 1];
        }
        double R = (x - _xmin) / _deltax;
        index = (int)(R);
        double offset = R - index;
        return _y[index] + (_y[index + 1] - _y[index]) * offset;
    }
    double GetFunction(double x) {
        if (x <= _xmin) {
             return _y[0];
        }
        if (x >= _xmax) {
            return _y[_y.size() - 1];
        }
        double R = (x - _xmin) / _deltax;
        int index = (int)(R);
        double offset = R - index;
        return _y[index] + (_y[index + 1] - _y[index]) * offset;
    }
    void IncrementPoints() {
        int points = (int)_y.size() + 1;
        double deltax = (_xmax - _xmin) / (points - 1);
        std::vector<double> y(points);
        y[0] = _y[0];
        y[points - 1] = _y[_y.size() - 1];
        for (int i = 1; i < points - 1; ++i) {
            y[i] = GetFunction(_xmin + i * deltax);
        }
        _deltax = deltax;
        _y.clear();
        for (int i = 0; i < y.size(); i++)
        {
            _y.push_back(y[i]);
        }
    }
private:
    double _xmin, _xmax, _deltax;
    std::vector<double> _y;
    void SetLimits() {
        double range = _xmax - _xmin;
        _xmin -= 0.01 * range;
        _xmax += 0.01 * range;
        _deltax = (_xmax - _xmin) / (_y.size() - 1);
    }
    void SetRandomFunction(double ymin, double ymax, int points) {
        _y = std::vector<double>(points);
        for (int i = 0; i < points; ++i) {
            double value = rand() % 1000 / 1000.0 * (ymax - ymin) + ymin;
            _y[i] = value;
        }
    }
    void FitDefinition(double x) {
        if (x < _xmin) {
            _xmin = x;
            SetLimits();
        }
        if (x > _xmax) {
            _xmax = x;
            SetLimits();
        }
    }
};
