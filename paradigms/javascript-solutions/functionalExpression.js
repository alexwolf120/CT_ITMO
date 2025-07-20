"use strict";
const VARIABLES = {
    x: 0,
    y: 1,
    z: 2,
};
const variable = name => (...values) => {
    return values[VARIABLES[name]]
};
const operation = binoper => (a,b) => (x, y, z) => binoper(a(x,y,z), b(x,y,z))
const unaroperation = unoper => (a) => (x, y, z) => unoper(a(x,y,z))
const cnst = value => () => value;

const add = operation((a, b) => a + b);
const subtract = operation((a, b) => a - b);
const multiply = operation((a, b) => a * b);
const divide = operation((a, b) => a / b);
const negate = unaroperation(a => -a);
const sinh = unaroperation(a => Math.sinh(a))
const cosh = unaroperation(a => Math.cosh(a))
const one = cnst(1)
const two = cnst(2)
const testing = function (n) {
    for (let i = 0; i < 10; i++){
        console.log(add(subtract(multiply(variable(x),variable(x)), multiply(two, variable(a))), one))
    }
};
//testing(10)

//TestJS.cmd jstest.functional.FunctionalTest easy OneSinhCosh