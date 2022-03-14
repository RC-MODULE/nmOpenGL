#ifndef IOBSERVER_H
#define IOBSERVER_H
#include <list>
#include <map>
#include <functional>

class IObserver {
public:
  virtual ~IObserver(){};
  virtual void update() = 0;
};


class ISubject {
public:
    virtual ~ISubject(){};
    virtual void attach(IObserver *observer) = 0;
    virtual void attach(const std::function<void()> observer) = 0;

    virtual void detach(IObserver *observer) = 0;

    virtual void notify() = 0;
};

class Subject : public ISubject{
private:

    std::list<IObserver *> list_observer;
    std::list<std::function<void()>> list_observer_lambda;
public:
    Subject(){
    }

    ~Subject() override {

    }

    void attach(IObserver *observer) override{
        list_observer.push_back(observer);
    }
    void detach(IObserver *observer) override{
        list_observer.remove(observer);
    }
    void attach(const std::function<void()> observer) override{
        list_observer_lambda.push_back(observer);
    }

    void notify() override{
        for(auto i : list_observer){
            i->update();
        }
        for(auto func : list_observer_lambda){
            func();
        }
    }


};



#endif // IOBSERVER_H
