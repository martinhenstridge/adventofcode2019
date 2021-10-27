#!/usr/local/bin/csi -ss

(import (chicken string))


(define (string->range s)
  (map string->number (string-split s "-")))


(define (decreasing? password)
  (let loop ((curr password))
    (if
      (= curr 0)
      #f
      (let ((next (quotient curr 10)))
        (if
          (< (remainder curr 10) (remainder next 10))
          #t
          (loop next))))))


(define (classify-password password)
  (let loop
    ((curr-digits password)
     (any-streak #f)
     (streak-len 1))
    (if
      (= curr-digits 0)
      (cons any-streak (= streak-len 2))
      (let ((next-digits (quotient curr-digits 10)))
        (let
          ((curr-digit (remainder curr-digits 10))
           (next-digit (remainder next-digits 10)))
          (if
            (= curr-digit next-digit)
            (loop next-digits #t (+ 1 streak-len))
            (if
              (= streak-len 2)
              (cons #t #t)
              (loop next-digits any-streak 1))))))))


(define (count-valid-passwords range)
  (let ((lower (car range))
        (upper (cadr range)))
    (let loop
      ((password lower)
       (count-one 0)
       (count-two 0))
      (if
        (> password upper)
        (cons count-one count-two)
        (if
          (decreasing? password)
          (loop (+ 1 password) count-one count-two)
          (let ((info (classify-password password)))
            (loop
              (+ 1 password)
              (+ (if (car info) 1 0) count-one)
              (+ (if (cdr info) 1 0) count-two))))))))


(define INPUT "271973-785961")
(define (main args)
  (let ((range (string->range INPUT)))
    (let ((counts (count-valid-passwords range)))
      (print (car counts))     ; 925
      (print (cdr counts)))))  ; 607
