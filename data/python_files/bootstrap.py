R"(
from core import *
from sq import *
import pyqpp as pq
import rlcompleter
import sys
import embc
import cad

ws = get_wsm()

sys.stdout = output_redirector()
cm = rlcompleter.Completer()

def complete_text(text):
        ret_list = []
        for i in range(0,40):
                x = cm.complete(text, i)
                if x is None:
                        break
                else:
                        ret_list.append(x)
        return ret_list

def input_wrapper():
    print("input(...) is blocked")

input = input_wrapper

)"

