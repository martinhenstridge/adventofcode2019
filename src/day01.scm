#!/usr/local/bin/csi -s

(import (chicken io))
(import srfi-1)
(import srfi-13)


(define (file->list path)
  (call-with-input-file path read-lines))


(define (parse-input lines)
  (map string->number (remove string-null? lines)))


(define (mass->fuel m)
  (- (quotient m 3) 2))


(define (combined-mass->fuel m)
  (let loop ((total 0) (curr m))
    (let ((fuel (mass->fuel curr)))
      (if (<= fuel 0)
      total
      (loop (+ total fuel) fuel)))))


(define (part-one ms)
  (foldl + 0 (map mass->fuel ms)))


(define (part-two ms)
  (foldl + 0 (map combined-mass->fuel ms)))


(define INPUT (file->list "data/01.txt"))
(define (main)
  (let ((ms (parse-input INPUT)))
    (print (part-one ms))   ; 3520097
    (print (part-two ms)))) ; 5277255


(main)
