#!/usr/local/bin/csi -ss

(import (chicken string))


(define (string->range s)
  (map string->number (string-split s "-")))


(define (number->digits num)
  (let loop ((n num) (digits '()))
    (if
      (= n 0)
      digits
      (loop (quotient n 10) (cons (remainder n 10) digits)))))


(define (never-decreasing? digits)
  (let loop
    ((head (car digits))
     (tail (cdr digits)))
    (if
      (null? tail)
      #t
      (if
        (< (car tail) head)
        #f
        (loop (car tail) (cdr tail))))))


(define (repeats-atleast-twice? digits)
  (let loop
    ((head (car digits))
     (tail (cdr digits)))
    (if
      (null? tail)
      #f
      (if
        (= (car tail) head)
        #t
        (loop (car tail) (cdr tail))))))


(define (repeats-exactly-twice? digits)
  (let loop
    ((head (car digits))
     (tail (cdr digits))
     (streak 1))
    (if
      (null? tail)
      (= streak 2)
      (if
        (= (car tail) head)
        (loop (car tail) (cdr tail) (+ 1 streak))
        (if
          (= streak 2)
          #t
          (loop (car tail) (cdr tail) 1))))))


(define (count-valid-passwords range valid?)
  (let ((lower (car range))
        (upper (cadr range)))
    (let loop ((n lower) (results '()))
      (if
        (> n upper)
        (length results)
        (if
          (valid? n)
          (loop (+ n 1) (cons n results))
          (loop (+ n 1) results))))))


(define (part-one range)
  (define (valid-password? password)
    (let ((digits (number->digits password)))
      (and
        (never-decreasing? digits)
        (repeats-atleast-twice? digits))))
  (count-valid-passwords range valid-password?))


(define (part-two range)
  (define (valid-password? password)
    (let ((digits (number->digits password)))
      (and
        (never-decreasing? digits)
        (repeats-exactly-twice? digits))))
  (count-valid-passwords range valid-password?))


(define INPUT "271973-785961")
(define (main args)
  (let ((range (string->range INPUT)))
    (print (part-one range))   ; 925
    (print (part-two range)))) ; 607
