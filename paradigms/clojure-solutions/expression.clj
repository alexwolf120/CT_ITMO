(defn variable [vars] (fn [varib] (varib vars)))
(def constant constantly)
(defn op [f]
      (fn
        ([a] (fn [args] (f (a args))))
        ([a b] (fn [args] (f (a args) (b args))))))
(def add (op +))
(def subtract (op -))
(def negate (op -))
(def arcTan (op (fn [x] (Math/atan x))))
(def arcTan2 (op (fn [x y] (Math/atan2 x y))))
(def multiply (op *))
(def divide (op (fn [x y] (/ (double x) (double y)))))
(def parsmap {'+        add,
              '-        subtract,
              '*        multiply,
              '/        divide,
              'negate   negate,
              'atan     arcTan,
              'atan2    arcTan2,
              })
(defn parseFunction [expression]
      (letfn [(parse [expr] (cond (symbol? expr)(variable (str expr))
                                  (number? expr) (constant expr)
                                  :else (apply (parsmap (first expr))(mapv parse (rest expr)))))]
             (parse (read-string expression))))
(defn proto-get
      ([obj key] (proto-get obj key nil))
      ([obj key default]
       (cond
         (contains? obj key) (obj key)
         (contains? obj :prototype) (proto-get (obj :prototype) key default)
         :else default)))

(defn proto-call
      [this key & args]
      (apply (proto-get this key) this args))

(defn field
      [key] (fn
              ([this] (proto-get this key))
              ([this def] (proto-get this key def))))

(defn method
      [key] (fn [this & args] (apply proto-call this key args)))
(defn constructor
      [ctor prototype]
      (fn [& args] (apply ctor {:prototype prototype} args)))

(def toString (method :toString))
(def evaluate (method :evaluate))
(def operands (field :operands))
(def getvalue (field :value))

(defn ConProto [const]
      {:toString  (fn [expr]
                      (str (getvalue expr)))
       :evaluate (fn
                   [expr vars]
                   (getvalue expr))
       :value     const})
(def Constant ConProto)
(defn VarProto [var]
      {:toString  (fn [expr]
                      (str (getvalue expr)))
       :evaluate (fn
                   [expr vars]
                   (vars (getvalue expr)))
       :value     var})
(def Variable VarProto)
(def symbol (field :opstr))
(def function (field :op))
(def OperProto {
                :toString  (fn [this] (str "(" (symbol this) " " (clojure.string/join " " (mapv toString (operands this))) ")"))
                 :evaluate (fn [this vars]
                               (apply (function this)
                                      (mapv (fn [oper] (evaluate oper vars))
                                            (operands this))))
                 })
(def create
  (fn [opstr op]
      (defn Oper [this & operands]
            (assoc (assoc (assoc this
                                 :op op)
                          :opstr opstr)
                   :operands operands)
            )
  (constructor Oper OperProto)))
(def Add
  (create '"+" +))

(def Subtract
  (create '"-" -))

(def Multiply
  (create '"*" *))
(def Divide
  (create '"/" (fn [x y] (/ (double x) (double y)))))
(def Negate
  (create '"negate" -
          ))
(def Sinh
  (create '"sinh" (fn [x] (Math/sinh x))
          ))
(def Cosh
  (create '"cosh" (fn [x] (Math/cosh x))
          ))

(def parsMapObj {'+      Add,
                 '-      Subtract,
                 '*      Multiply,
                 '/      Divide,
                 'negate Negate,
                 'sinh   Sinh,
                 'cosh   Cosh})

(defn parseObject [expression]
      (letfn [(parse [expr] (cond (symbol? expr) (Variable (str expr))
                                  (number? expr) (Constant expr)
                                  :else (apply (parsMapObj (first expr))(mapv parse (rest expr)))))]
             (parse (read-string expression))))