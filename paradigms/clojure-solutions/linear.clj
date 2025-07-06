(defn abstr [f]
      (fn [a b]
          (mapv f a b)))
(def v+ (abstr +))
(def v- (abstr -))
(def v* (abstr *))
(def vd (abstr /))
(defn scalar [A B]
      (apply + (v* A B)))
(defn vect [a, b] [(- (* (get a 1) (get b 2)) (* (get a 2) (get b 1))),
                   (- (* (get a 2) (get b 0)) (* (get a 0) (get b 2))),
                   (- (* (get a 0) (get b 1)) (* (get a 1) (get b 0)))])
(defn v*s [a, s] (mapv (fn [f] (* s f)) a))
(def m+ (abstr v+ ))
(def m- (abstr v- ))
(def m* (abstr v* ))
(def md (abstr vd ))
(defn transpose [m]
      (apply mapv vector m))
(defn m*s [a s] (mapv #(v*s % s) a))
(defn m*v [a c] (mapv #(scalar % c) a))
(defn m*m [a b] (transpose (mapv #(m*v a %) (transpose b))))
(defn Shapeless [f]
      (fn shape [a b](if(number? a)
                       (do (f a b))
                       (do (mapv shape a b)))))
(def s+ (Shapeless +))
(def s- (Shapeless -))
(def s* (Shapeless *))
(def sd (Shapeless /))
