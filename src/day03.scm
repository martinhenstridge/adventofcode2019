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
         update-position
         record-position
         starting-position
         starting-distance
         segment-len)
  (let loop ((position starting-position) (steps 0))
    (if
      (= steps segment-len)
      position
      (let ((updated (update-position position)))
        (record-position updated (+ starting-distance steps 1))
        (loop updated (+ steps 1))))))


(define (process-wire wire record-position)
  (let loop ((remaining wire)
             (starting-position (cons 0 0))
             (starting-distance 0))
    (if
      (null? remaining)
      '()
      (let*
        ((segment (car remaining))
         (segment-dir (car segment))
         (segment-len (cdr segment))
         (update-position (make-update-position-fn segment-dir))
         (updated-position (process-segment
                            update-position
                            record-position
                            starting-position
                            starting-distance
                            segment-len)))
        (loop (cdr remaining) updated-position (+ starting-distance segment-len))))))


(define (manhattan position)
  (+ (abs (car position)) (abs (cdr position))))


(define (calculate-layout wires)
  (define layout (make-hash-table))
  (process-wire
    (list-ref wires 0)
    (make-record-position-fn
      layout
      (lambda (record distance)
        (if (car record)
            record
            (cons distance (cdr record))))))
  (process-wire
    (list-ref wires 1)
    (make-record-position-fn
      layout
      (lambda (record distance)
        (if (cdr record)
            record
            (cons (car record) distance)))))
  layout)


(define (part-one layout)
  (car
    (sort
      (map
        manhattan
        (hash-table-fold layout
          (lambda (key val acc)
            (if
              (and (car val) (cdr val))
              (cons key acc)
              acc))
          '()))
      <)))


(define (part-two layout)
  (car
    (sort
      (hash-table-fold
        layout
        (lambda (key val acc)
          (if
            (and (car val) (cdr val))
            (cons (+ (car val) (cdr val)) acc)
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
