/*
[Please do not mistake this as advocating for any specific implementation.  It's intent was
to facilitate communication and provide examples of alternate approaches.]

Many moons ago, I was a junior programmer and I was told by a senior engineer about how terrible 
dynamic_cast was; How the sky would rain fire if I ever used it.  So, I never did.  Every place 
I've worked since has had a moratorium on its use.  I've never had a reason to question it, so 
when I saw it being used a lot in a recent code base, I was surprised.  Now I had a reason to 
question that sage old advice, so I ran some tests.

Turns out it is "slow".  Tests show it to be 15x to 50x slower than a simple, naive ID system 
that relies on virtual functions to do a simple cast.  Defining "slow" is important, though.  
We're talking about 30-50 nanoseconds per operation (Averaged over 1000 iterations).  Enough that 
doing it in a tight loops matters but if you are dynamic_casting in a tight loop, there are 
bigger issues.

This leads me into another thing I've learned over the years but not something anyone ever told me:  
When people start talking about "speed" of code I get pretty suspicious.  Call me cynical but I find 
that sort of talk to be a smoke screen excuse about why using their code is obtuse and difficult.  
This isn't to suggest speed doesn't matter, obviously it does.  
But I find it usually falls into one of two categories: 
    1) Obvious: It is going to matter.  Code for it and document the design decisions/trade offs.
    2) Not obvious: Nobody knows until its profiled. Optimize when there is a problem.

Now, to be fair; dynamic_cast is doing more work than the implementation here.  As it will cast an 
ancestor to any decedent along the path to the concrete type.  The attached source will only cast 
to the concrete type.  But, how often do you need that extra functionality  and is it worth the 
cost trade off(1)?  This idea of full functionality versus good enough is one I will reference in 
the future.  
Metaphorically: Use the tool that does the job, not the tool that does all the jobs.



Casting up/down the hierarchy to simply call a virtual function is one thing.  Real code tends to 
be more complex.  The  code that put on this line of thinking was more along the lines of the 
GrandParent/GrandParent::ObjectData, LeftParent/LeftParent::ParentData in the example code (Reuse.cpp).
It relies heavily on inheritance for code reuse.  Code reuse is great!  Inheritance is an easy way to 
express code reuse.  The down side is that it can paint us into a corner.  It can not only forces us to 
use some form of down casting (dynamic_cast) but also the "Call Super" anti-pattern.  
(https://en.wikipedia.org/wiki/Call_super)
The danger here is in confusing the Is-A relationship of inheritance and the algorithmic enforcement 
of a specific interface.  This is the classic battle between inheritance and composition.  Those don't 
have to be the only options.

Consider the code in Reuse.cpp - The base class "Parent" is abstract.  Its real purpose is a bit hard 
to divine.  It exists to enforce an algorithm, same as the original source.  The algorithm is that 
when you process an object, you do Step1, then Step2 then Render, with the conceit that Step1 and 
Step2 are specific to the concrete classes.  It also has some common functionality that it exposes 
for code reuse (which we've established is great!).  The problem is, to do this effectively we have 
to know what the ObjectData concrete type is.  This breaks the Liskov substitution principal 
(https://en.wikipedia.org/wiki/Liskov_substitution_principle) and should be a red flag(2).

Now consider the code in Reuse2.cpp.  It is very similar to that of the original Reuse case but instead 
of having a Process() function on the base class that boot straps the algorithm, it implements an 
interface(3) that moves the abstraction to the children classes.  This is the opposite of code reuse 
(which we established is great!).  It re-implements the algorithm in two places.  ABATO!(1)  We 
re-implement the algorithm, essentially making it even more explicit (good) but simplify the data 
migration across objects.  I would argue the algorithm here is pretty simple:  Step1 --> Step2 --> Render!  
But the data flow is more complex; we need to know which object we are dealing with to determine what 
work to do. Is it a duck or a goose?

This horse isn't quiet dead, so take a look at Reuse3.cpp.  Astute observers might have noticed that 
the Step1/Step2 functions were not actually mutating the Parent object.  So, why are they even members 
of the object?  Great question!  In the eternal struggle to be IRobust (4) they don't need to be.  In 
this 3rd example they are static members of the class.  They could be static local scope functions too, 
I made them like this for continuity of earlier examples.  Either way they explicitly state they do not 
mutate the object.  They simply exist for code reuse (which we established is great!).  



In summary:  Code reuse != inheritance != composition.  Disambiguate algorithm from data.  
Object Oriented programming at its heart is about bundling data and transformations together.  
If your function isn't strictly about mutating the object it is on, consider alternatives.



1) Always Be Asking Trade Offs (ABATO).  This isn't a thing. I made it up.  Now it is a thing.
2) Code smells are not wrong per se and are *sometimes* are the best approach.  But if you see/smell 
    one, it is a signal that there might be a better approach.  (https://en.wikipedia.org/wiki/Code_smell)  
3) Interface isn't a C++ concept.  It is a #define in this example that equates to class.  
    It is nothing more than a shorthand letting the reader know that "This object has no implementation 
    and is pure abstract".  Readability matters.
4) Robust code - The fastest, most maintainable, stable code is no code.  This sounds like a joke but is a 
    serious point with regards to software development.  Strive for the most minimal solution possible.


*/