package expression.exceptions;

import expression.*;
import expression.parser.TripleParser;

import java.util.ArrayList;
import java.util.List;

import static expression.CollectNum.ColNum;

public class ExpressionParser implements TripleParser {

    public TripleExpression parse(String expression){
        List<Symbol> symbols = null;
        try {
            symbols = SymAnalizator(expression);
        } catch (ParsExp e) {
            throw new RuntimeException(e);
        }
        SymBuf symBuf = new SymBuf(symbols);
        try {
            return pars(symBuf);
        } catch (ParsExp e) {
            throw new RuntimeException(e);
        }
    }
    public static Supint pars(SymBuf lexemes) throws ParsExp{
        Symbol lexeme = lexemes.next();
        if (lexeme.type == Symbolstype.eof) {
            return null;
        } else {
            lexemes.back();
            return addsub(lexemes);
        }
    }

    public static class Symbol {
        public Symbolstype type;
        public String value;
        public Symbol(Symbolstype type, Character value) {
            this.type = type;
            this.value = value.toString();
        }
        public Symbol(Symbolstype type, String value) {
            this.type = type;
            this.value = value;
        }
    }

    public static class SymBuf {
        private int pos;
        public List<Symbol> Symbols;
        public SymBuf(List<Symbol> Symbols) {
            this.Symbols = Symbols;
        }
        public Symbol next() {
            return Symbols.get(pos++);
        }
        public void back() {
            pos--;
        }
    }

    public static List<Symbol> SymAnalizator(String str) throws ParsExp {
        ArrayList<Symbol> symbols = new ArrayList<>();
        int ind = 0;
        int balance = 0;
        boolean op = true;
        str += " ";
        while (ind< str.length()) {
            char c = str.charAt(ind);
            char k;
            switch (c) {
                case 'x', 'y', 'z' -> {
                    symbols.add(new Symbol(Symbolstype.var, c));
                    ind++;
                    op = false;
                }
                case '(' -> {
                    symbols.add(new Symbol(Symbolstype.open_br, c));
                    ind++;
                    balance++;
                    op = true;
                }
                case ')' -> {
                    symbols.add(new Symbol(Symbolstype.close_br, c));
                    ind++;
                    balance--;
                    if (balance < 0) {
                        throw new Bracket("Not enough opening bracket(s)");
                    }
                    op = false;
                }
                case '+' -> {
                    symbols.add(new Symbol(Symbolstype.plus, c));
                    ind++;
                    op = true;
                }
                case '-' -> {
                    StringBuilder sb = new StringBuilder();
                    k = str.charAt(ind + 1);
                    if (op && k <= '9' && k >= '0' && !Character.isWhitespace(k)) {
                        sb.append(c);
                        ind++;
                        c = str.charAt(ind);
                        sb.append(ColNum(ind,c,str, '9','0'));
                        try {
                            Integer.parseInt(sb.toString());
                        }catch (NumberFormatException e){
                            throw new OverFlowExc("Overflow down");
                        }
                        symbols.add(new Symbol(Symbolstype.num, sb.toString()));
                    } else {
                        symbols.add(new Symbol(Symbolstype.minus, c));
                        ind++;
                        op = true;
                    }
                }
                case '*' -> {
                    symbols.add(new Symbol(Symbolstype.mult, c));
                    ind++;
                    op = true;
                }
                case '/' -> {
                    symbols.add(new Symbol(Symbolstype.divide, c));
                    ind++;
                    op = true;
                }
                default -> {
                    StringBuilder sb = new StringBuilder();
                    if (c <= '9' && c >= '0') {
                        while (c <= '9' && c >= '0'){
                            sb.append(c);
                            ind++;
                            if (ind >= str.length()) break;
                            c = str.charAt(ind);
                        }
                        symbols.add(new Symbol(Symbolstype.num, sb.toString()));
                        if(Character.isWhitespace(c)){
                            while (Character.isWhitespace(c)){
                                ind++;
                                if (ind >= str.length())break;
                                c = str.charAt(ind);
                            }
                            if(c <= '9' && c >= '0'){
                                throw new IllegalArgumentException("Double num");
                            }
                        }
                    }else if(c == 'c' || c == 's'){
                        while (c <= 'z' && c >= '0') {
                            sb.append(c);
                            ind++;
                            if (ind >= str.length()) break;
                            c = str.charAt(ind);
                        }
                        switch (sb.toString()) {
                            case "count" -> symbols.add(new Symbol(Symbolstype.count, sb.toString()));
                            case "clear" -> {
                                symbols.add(new Symbol(Symbolstype.clear, sb.toString()));
                                op = true;
                            }
                            case "set" -> {
                                symbols.add(new Symbol(Symbolstype.set, sb.toString()));
                                op = true;
                            }
                            default -> throw new IllegalArgumentException("not count or clear or set");
                        }
                    }else{
                        if (Character.isWhitespace(c)) {
                            ind++;
                        } else {
                            throw new IllegalArgumentException("Invalid symbol");
                        }
                    }
                }
            }
        }
        symbols.add(new Symbol(Symbolstype.eof, ""));
        if (balance != 0) {
            throw new Bracket("No enough closing bracket(s)");
        }
        return symbols;
    }

    public static Supint firstp(SymBuf lexemes) throws ParsExp {
        Symbol lexeme = lexemes.next();
        switch (lexeme.type) {
            case num -> {
                try {
                    Integer.parseInt(lexeme.value);
                }catch (NumberFormatException e){
                    throw new OverFlowExc("Overflow up");
                }
                return new Const(Integer.parseInt(lexeme.value));
            }
            case var -> {
                return new Variable(lexeme.value);
            }
            case minus -> {
                return new CheckedNegate(firstp(lexemes));
            }
            case count -> {
                return new Count(firstp(lexemes));
            }
            case open_br -> {
                Supint value = addsub(lexemes);
                lexeme = lexemes.next();
                return value;
            }
            case mult,plus,set,clear,divide -> throw new ParsExp("Incorrect operation");
            default -> {
                throw new ParsExp("No operation argument");
            }
        }
    }
    public static Supint muldiv(SymBuf lexemes) throws ParsExp {
        Supint value = firstp(lexemes);
        while (true) {
            Symbol lexeme = lexemes.next();
            switch (lexeme.type) {
                case mult -> value = new CheckedMultiply(value, firstp(lexemes));
                case divide -> value = new CheckedDivide(value, firstp(lexemes));
                case eof, close_br,plus, minus,set,clear-> {
                    lexemes.back();
                    return value;
                }
            }
        }
    }
    public static Supint addsub(SymBuf lexemes) throws ParsExp {
        Supint value = muldiv(lexemes);
        while (true) {
            Symbol lexeme = lexemes.next();
            switch (lexeme.type) {
                case plus -> value = new CheckedAdd(value, muldiv(lexemes));
                case minus -> value = new CheckedSubtract(value, muldiv(lexemes));
                case set -> value = new Set(value, muldiv(lexemes));
                case clear -> value = new Clear(value, muldiv(lexemes));
                case eof, close_br,num-> {
                    lexemes.back();
                    return value;
                }
            }
        }
    }

}