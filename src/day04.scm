#!/usr/local/bin/guile -s
!#


(define (string->range s)
  (map string->number (string-split s #\-)))


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


(define (generate-never-decreasing range)
  (let ((lower (car range))
        (upper (cadr range)))
    (let loop ((password lower) (passwords '()))
      (if
        (> password upper)
        passwords
        (if
          (decreasing? password)
          (loop (+ 1 password) passwords)
          (loop (+ 1 password) (cons password passwords)))))))


(define (repeats-atleast-twice? password)
  (let loop ((curr-digits password))
    (if
      (= curr-digits 0)
      #f
      (let ((next-digits (quotient curr-digits 10)))
        (let
          ((curr-digit (remainder curr-digits 10))
           (next-digit (remainder next-digits 10)))
          (if
            (= curr-digit next-digit)
            #t
            (loop next-digits)))))))


(define (repeats-exactly-twice? password)
  (let loop
    ((curr-digits password)
     (streak-len 1))
    (if
      (= curr-digits 0)
      (= streak-len 2)
      (let ((next-digits (quotient curr-digits 10)))
        (let
          ((curr-digit (remainder curr-digits 10))
           (next-digit (remainder next-digits 10)))
          (if
            (= curr-digit next-digit)
            (loop next-digits (+ 1 streak-len))
            (if
              (= streak-len 2)
              #t
              (loop next-digits 1))))))))


(define (part-one candidates)
  (length (filter repeats-atleast-twice? candidates)))


(define (part-two candidates)
  (length (filter repeats-exactly-twice? candidates)))


(define INPUT "271973-785961")
(define (main)
  (let ((range (string->range INPUT)))
    (let ((candidates (generate-never-decreasing range)))
      (display (part-one candidates)) ; 925
      (newline)
      (display (part-two candidates)) ; 607
      (newline))))


(main)
