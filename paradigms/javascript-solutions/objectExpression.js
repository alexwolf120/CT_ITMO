"use strict";
function Op(operation, func,...args) {
    this.operation = operation
    this.func = func
    this.args = args
    Op.prototype.evaluate = function (...args) {
        return this.func(...this.args.map(arg => arg.evaluate(...args)))
    }
    Op.prototype.toString = function () {
        return this.args.map(x => x.toString()).join(" ") + " " + this.operation;
    }
    Op.prototype.prefix = function (){
        return "(" + this.operation + " " + this.args.map(x => x.prefix()).join(" ") + ")";
    }
}
function CreateOp(operation, func) {
    function Operation(...args) {
        Op.call(this, operation, func, ...args)
    }
    Operation.prototype = Object.create(Op.prototype)
    parsmap[operation] = [Operation, func.length]
    return Operation
}

const parsmap = {}
const Add = CreateOp('+',
    (a, b) => a + b
)

const  Subtract = CreateOp( '-',
    (a, b) => a - b,
)

const Multiply = CreateOp( '*',
    (a, b) => a * b
)

const Divide = CreateOp( '/',
    (a, b) => a / b
)
const Negate = CreateOp( 'negate',
    (a) => -a
)

const ArcTan = CreateOp( 'atan',
    (a) => Math.atan(a),
)

const ArcTan2 = CreateOp( 'atan2',
    (a,b) => Math.atan2(a,b),
)
const Sum = CreateOp( 'sum',
        function (...args) {
            if (args.length === 0) {
                return 0;
            }
            return args.reduce((a, b) => (a + b));
        }
)
const Avg = CreateOp( 'avg',
    function (...args) {
        if (args.length === 0) {
            return 0;
        }
        return args.reduce((a, b) => (a + b)) / args.length;
    }
)
const VARIABLES = {
    "x": 0,
    "y": 1,
    "z": 2
}
function Variable(variable) {
    this.evaluate = (...args) => args[VARIABLES[variable]];
    this.toString = () => variable;
    this.prefix = () =>variable ;
}
function Const(cnst) {
    this.evaluate = () => cnst;
    this.toString = () => cnst.toString();
    this.prefix = () => cnst.toString();
}
const parse = str =>{
    const stack = [];
    str = str.replace(/ + /g, " ").trim().split(" ")
    for (const operand of str){
        if(operand in parsmap){
            const args = []
            for (let i = 0; i < parsmap[operand][1]; i++) {
                args.unshift(stack.pop())
            }
            stack.push(new parsmap[operand][0](...args));
        }else{
            stack.push(operand === "x" || operand === "y" || operand === "z" ? new Variable(operand) : new Const(parseFloat(operand)));
        }
    }
    return stack.pop();
}
function ParseError(error) {
    Error.call(this, error);
    this.error = error;
}

const parsePrefix = str => {
    let brakes = 0;
    str = str.replace(/\(/g, " ( ").replace(/\)/g, " ) ").replace(/ + /g, " ").trim().split(" ");
    if (str.length === 0 || str[0] === "") {
        throw new ParseError("Empty input");
    }
    let k = 0;
    let args = [];
    for (let op = str.length-1; op >= 0; op--){
        if (str[op] === ')') {
            brakes++
        }else if(str[op] === "("){
            if(!(str[op+1] in parsmap)){
                throw new ParseError("Error operation")
            }
            brakes--
            if(brakes < 0){
                throw new ParseError("not many brakes")
            }
        }else if (str[op] in parsmap){
            if(str[op-1] !== "(" && brakes !== 0 || op === 0){
                throw new ParseError("Error operation")
            }
            let stack = [];
            if(str[op+1] === "(" && !(str[op+2] in parsmap)){
                throw new ParseError("1Error operation")
            }
            if(str[op+1] === ")" && (str[op] === "sum" || str[op] === "avg")) {
                args.push(new parsmap[str[op]][0]);
            }
            else if(str[op+1] === ")" && (str[op] !== "sum" || str[op] !== "avg")) {
                throw new ParseError("No arguments");
            }
            else{
                let kar = 0;
                let br = 0;
                let i = op + 1
                while(br >= 0 && i < str.length){
                    if (str[i] === "("){
                        br++;
                        k = 1;
                        while(k !== 0){
                            if(str[i+1] === ")")k--;
                            else if(str[i+1] === "(")k++;
                            i++
                        }
                        i--;
                    }else if(str[i] === ")"){
                        br--
                        if(br < 0){
                            break
                        }
                        kar++
                    }else{
                        kar++;
                    }
                    i++;
                }
                for(let i = 0; i < kar; i++){
                    stack.push(args.pop())
                }
                if(str[op] !== "sum" && str[op] !== "avg" && kar !== parsmap[str[op]][1]){
                    throw new ParseError("More arguments");
                }
                args.push(new parsmap[str[op]][0](...stack));
            }
        }else if (!isNaN(str[op])){
            args.push(new Const(parseInt(str[op])))
        }else if (str[op] in VARIABLES){
            args.push(new Variable(str[op]));
        }else{
            throw new ParseError("Undefined object");
        }
    }
    if(brakes !== 0){
        throw new ParseError("not many brakes")
    }
    if (args.length !== 1){
        throw new ParseError("Incorrect number of arguments")
    }
    return args.pop()
}
//console.log(parsePrefix("(negate)").prefix())
// TestJS.cmd jstest.prefix.ParserTest easy SumAvg
// cd WebstormProjects\hw6\paradigms-2023\javascript
