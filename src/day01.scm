#!/usr/local/bin/guile -s
!#

(use-modules (srfi srfi-1))
(use-modules (ice-9 textual-ports))


(define (file->list path)
  (string-split (call-with-input-file path get-string-all) #\newline))


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
  (fold + 0 (map mass->fuel ms)))


(define (part-two ms)
  (fold + 0 (map combined-mass->fuel ms)))


(define INPUT (file->list "data/01.txt"))
(define (main)
  (let ((ms (parse-input INPUT)))
    (display (part-one ms)) ; 3520097
    (newline)
    (display (part-two ms)) ; 5277255
    (newline)))


(main)
