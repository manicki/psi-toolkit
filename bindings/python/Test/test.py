# -*- coding: UTF-8 -*-

import PSIToolkit
import unittest
import nose

class PSIToolkitTest(unittest.TestCase):

    def test_1(self):    
        psi = PSIToolkit.PipeRunner('tp-tokenizer --lang pl')
        output = psi.run('Pan prof. dr hab. Jan Nowak.')
        expected = 'Pan\nprof.\ndr\nhab.\nJan\nNowak\n.\n'
        
        self.assertEqual(expected, output)

    def test_2(self):    
      psi = PSIToolkit.PipeRunner('tp-tokenizer --lang pl')
      output = psi.run('ĄŻŚŹĘĆÓŁŃ ążśźęćółń')
      expected = 'ĄŻŚŹĘĆÓŁŃ\nążśźęćółń\n'
      
      self.assertEqual(expected, output)
      
    def test_3(self):    
      psi = PSIToolkit.PipeRunner('tp-tokenizer --lang pl ! srx-segmenter --lang pl ! simple-writer --tag segment')
      output = psi.run('Ala ma kota. Kot ma Ale.')
      expected = 'Ala ma kota.\n Kot ma Ale.\n'
      
      self.assertEqual(expected, output)  
      
if __name__ == '__main__':
    nose.main() 
