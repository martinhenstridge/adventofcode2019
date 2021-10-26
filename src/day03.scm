#!/usr/local/bin/csi -ss

(import (chicken io))
(import (chicken sort))
(import (chicken string))
(import (chicken bitwise))
(import srfi-1)
(import srfi-13)
(import srfi-48)
(import srfi-69)


(define (file->list path)
  (call-with-input-file path read-lines))


(define (string->wire line)
  (map
    (lambda (s)
      (cons
        (string-take s 1)
        (string->number (string-drop s 1))))
    (string-split line ",")))


(define (parse-input lines)
  (map string->wire (remove string-null? lines)))


(define (make-update-position-fn direction)
  (cond
    ((string=? direction "U") (lambda (p) (cons (car p) (+ (cdr p) 1))))
    ((string=? direction "D") (lambda (p) (cons (car p) (- (cdr p) 1))))
    ((string=? direction "L") (lambda (p) (cons (- (car p) 1) (cdr p))))
    ((string=? direction "R") (lambda (p) (cons (+ (car p) 1) (cdr p))))))


(define (make-record-position-fn table update-entry)
  (lambda (position distance)
    (hash-table-update!/default
      table
      position
      (lambda (entry)
        (update-entry entry distance))
      (cons #f #f))))


(define (process-segment
         segment
         starting-position
         starting-distance
         record-position)
  (let*
    ((segment-dir (car segment))
     (segment-len (cdr segment))
     (update-position (make-update-position-fn segment-dir)))
    (let loop ((position starting-position) (steps 0))
      (if
        (= steps segment-len)
        (cons position steps)
        (let ((updated-position (update-position position)))
          (record-position updated-position (+ starting-distance steps 1))
          (loop updated-position (+ steps 1)))))))


(define (process-wire wire record-position)
  (let loop ((remaining wire)
             (starting-position (cons 0 0))
             (starting-distance 0))
    (if
      (null? remaining)
      '()
      (let*
        ((segment (car remaining))
         (result (process-segment
                  segment
                  starting-position
                  starting-distance
                  record-position))
          (updated-position (car result))
          (updated-distance (+ starting-distance (cdr result))))
        (loop (cdr remaining) updated-position updated-distance)))))


(define (calculate-layout wires)
  (define layout (make-hash-table))
  (process-wire
    (list-ref wires 0)
    (make-record-position-fn
      layout
      (lambda (entry distance)
        (if (car entry)
            entry
            (cons distance (cdr entry))))))
  (process-wire
    (list-ref wires 1)
    (make-record-position-fn
      layout
      (lambda (entry distance)
        (if (cdr entry)
            entry
            (cons (car entry) distance)))))
  layout)


(define (part-one layout)
  (define (manhattan position)
    (+ (abs (car position)) (abs (cdr position))))
  (car
    (sort
      (hash-table-fold layout
        (lambda (key val acc)
          (if
            (and (car val) (cdr val))
            (cons (manhattan key) acc)
            acc))
        '())
      <)))


(define (part-two layout)
  (define (combined-distance distances)
    (+ (car distances) (cdr distances)))
  (car
    (sort
      (hash-table-fold
        layout
        (lambda (key val acc)
          (if
            (and (car val) (cdr val))
            (cons (combined-distance val) acc)
            acc))
        '())
      <)))


;(define INPUT '("R8,U5,L5,D3" "U7,R6,D4,L4" ""))
;(define INPUT '("R75,D30,R83,U83,L12,D49,R71,U7,L72" "U62,R66,U55,R34,D71,R55,D58,R83" ""))
;(define INPUT '("R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51" "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7" ""))
(define INPUT (file->list "data/03.txt"))
(define (main args)
  (let ((wires (parse-input INPUT)))
    (let ((layout (calculate-layout wires)))
      (print (part-one layout))    ; 721
      (print (part-two layout))))) ; 7388
