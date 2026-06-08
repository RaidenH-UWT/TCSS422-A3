# 2026/06/08

Started the assignment after a one-day essay endevour.

Needed to clone a github repo...
but I don't remember the exact process on how I set it up on Windows.
Nor do I actually know how I might set it up on a Linux machine.

I took the easy route of just using my Windows OS outside of the VM to handle git.
Normally I use SSH which requires fiddling around with keys.

I see that my partner took the time to do the main part of the assignment.
Nice to see that we got it solved, but I also do want to contribute my part.

Raiden said the main part is working but the extra-credit wasn't implemented.
That's my goal: implement the extra credit.

---

Before I start, I should at least familiarize myself and run the basic `helloModule`.

I got the little process count sample to run. Now I got the idea of what's going on.

Raiden got a simple shell script set up to make and insert the module and log the kernal.

Seems my trick of git cloning by Windows made the `\n` into `\r\n` and that broke it.
Rewriting it works though so whatever.

---

okay I did the thing and it looks right.

---

found a bug that was doing weird undefined behavior

but now raiden points out that there is another bug:
I don't consider if the first page is contiguous or not.

---

found the solution: as raiden pointed out,
simplest was to check if the total pages is 1,
then we can surmise something about the first page:
* if the second page is non-contiguous, the first one is non-contiguous
* if the second page is contiguous, the first one is contiguous

---

and now it works and runs and huzzah!

> *it just works* -Todd Howard


