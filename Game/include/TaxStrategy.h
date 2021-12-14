//
// Created by tesserakt on 14.12.2021.
//

#pragma once

struct TaxStrategy {
    virtual double calcTax(double balance) = 0;
    virtual ~TaxStrategy()                 = default;
};

struct Percentage : TaxStrategy {
    double percent;

    double calcTax(double balance) override {
        return balance * percent + 1;
    }

    Percentage(double percent) : percent(percent) {}
};

struct Constant : TaxStrategy {
    double constant;

    double calcTax(double balance) override {
        return constant;
    }

    Constant(double constant) : constant(constant) {}
};