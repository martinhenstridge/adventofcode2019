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
  (let loop ((curr password))
    (if
      (= curr 0)
      #t
      (let ((next (quotient curr 10)))
        (if
          (< (remainder curr 10) (remainder next 10))
          #f
          (loop next))))))


(define (repeats-atleast-twice? password)
  (let loop ((curr password))
    (if
      (= curr 0)
      #f
      (let ((next (quotient curr 10)))
        (if
          (= (remainder curr 10) (remainder next 10))
          #t
          (loop next))))))


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
    (let loop ((password lower) (count 0))
      (if
        (> password upper)
        count
        (if
          (valid? password)
          (loop (+ password 1) (+ count 1))
          (loop (+ password 1) count))))))


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
