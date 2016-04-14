(print (mapcar (lambda (x) (list (item.feat x 'name) (item.feat x 'segment_end))) (utt.relation.items (SayText "Hey. I'm dumb bot. I'm here, I want to amuse you.") 'Segment)))

