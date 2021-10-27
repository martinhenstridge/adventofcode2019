#!/usr/local/bin/csi -ss

(import (chicken string))


(define (string->range s)
  (map string->number (string-split s "-")))


(define (password->digits password)
  (let loop ((n password) (digits '()))
    (if
      (= n 0)
      digits
      (loop (quotient n 10) (cons (remainder n 10) digits)))))


(define (never-decreasing? password)
  (let loop ((n password))
    (if
      (= n 0)
      #t
      (let*
        ((units (remainder n 10))
         (tens (/ (remainder (- n units) 100) 10)))
        (if
          (< units tens)
          #f
          (loop (quotient n 10)))))))


(define (repeats-atleast-twice? password)
  (define digits (password->digits password))
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


(define (repeats-exactly-twice? password)
  (define digits (password->digits password))
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
  (count-valid-passwords
   range
   (lambda (p)
     (and
      (never-decreasing? p)
      (repeats-atleast-twice? p)))))


(define (part-two range)
  (count-valid-passwords
   range
   (lambda (p)
     (and
      (never-decreasing? p)
      (repeats-exactly-twice? p)))))


(define INPUT "271973-785961")
(define (main args)
  (let ((range (string->range INPUT)))
    (print (part-one range))   ; 925
    (print (part-two range)))) ; 607
