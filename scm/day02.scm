#!/usr/local/bin/csi -s

(import (chicken io))
(import (chicken string))
(import srfi-1)
(import srfi-13)
(import srfi-48)
(import vector-lib)


(define (file->list path)
  (call-with-input-file path read-lines))


(define (parse-input lines)
  (list->vector (map string->number (string-split (car lines) ","))))


(define (run-intcode program noun verb)
  (let ((mem (vector-copy program)))
    (vector-set! mem 1 noun)
    (vector-set! mem 2 verb)
    (let step ((ptr 0))
      (let ((opcode (vector-ref mem ptr)))
        (cond
          ((= opcode 99) (vector-ref mem 0))
          ((= opcode 1) (step (run-intcode-add mem ptr)))
          ((= opcode 2) (step (run-intcode-mul mem ptr))))))))


(define (run-intcode-add mem ptr)
  (let ((p1 (vector-ref mem (+ ptr 1)))
        (p2 (vector-ref mem (+ ptr 2)))
        (p3 (vector-ref mem (+ ptr 3))))
    (vector-set! mem p3 (+ (vector-ref mem p1) (vector-ref mem p2)))
    (+ ptr 4)))


(define (run-intcode-mul mem ptr)
  (let ((p1 (vector-ref mem (+ ptr 1)))
        (p2 (vector-ref mem (+ ptr 2)))
        (p3 (vector-ref mem (+ ptr 3))))
    (vector-set! mem p3 (* (vector-ref mem p1) (vector-ref mem p2)))
    (+ ptr 4)))


(define (part-one program)
  ; To do this, before running the program, replace position 1 with the value 12
  ; and replace position 2 with the value 2.
  (run-intcode program 12 2))


(define (part-two program)
  ; Find the input noun and verb that cause the program to produce the output
  ; 19690720. What is 100 * noun + verb?
  (let loop ((noun 0) (verb 0))
    (let ((output (run-intcode program noun verb)))
      (if
        (= output 19690720)
        (+ verb (* noun 100))
        (if
          (= verb 99)
          (loop (+ noun 1) 0)
          (loop noun (+ verb 1)))))))


;(define INPUT '("1,9,10,3,2,3,11,0,99,30,40,50" ""))
;(define INPUT '("1,0,0,0,99" ""))
;(define INPUT '("2,3,0,3,99" ""))
;(define INPUT '("2,4,4,5,99,0" ""))
;(define INPUT '("1,1,1,4,99,5,6,0,99" ""))
(define INPUT (file->list "data/02.txt"))
(define (main)
  (let ((program (parse-input INPUT)))
    (print (part-one program))   ; 3706713
    (print (part-two program)))) ; 8609


(main)
