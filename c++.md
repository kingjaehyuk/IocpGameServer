# 기본

헤더에는 선언만 할 것

`#pragma once` 중복 include 방지

<!-- 출력
Manipulator
showbase << hex << 10 << endl; // 10을 16진수로 출력
showpos/noshowpos // 양수 부호 출력
dec/hex/oct // 진수 출력(10, 16, 8)
uppercase/nouppercase // 대문자 출력
showbase/noshowbase // 몇진법인지 출력 -->

# 입력 스트림

**추상화되어있어서 파일스트림에서도 사용함**

istream 상태

- 네임스페이스 - ios_base
- 비트플래그, 메소드
  - goodbit, good()
  - eofbit, eof()
  - failbit, fail()
  - badbit, bad()

get()

- `\n`을 만나기 직전까지의 모든 문자를 가져옴
- `\n`은 입력 스트림에 남아있음

getline()

- get()과 차이점은 구분자(`\n`)를 입력 스트림에서 버림

```cpp
// 99개의 문자를 가져오거나 \n가 나올 때 까지의 문자를 가져오고
// 가져온 문자를 char배열에 배치함
get(firstName, 100);
getline(firstName, 100);

// 99개의 문자를 가져오거나 #이 나올 때 까지의 문자를 가져오고
// 가져온 문자를 char배열에 배치함
get(firstName, 100, '#');
getline(firstName, 100, '#');
```

# 참조

- `void Func(int num)`은 값에 의한 호출로 `num의 값 복사`해온 것으로 `수정`하면 `Func함수의 num값이 변경`되고 끝나면 스택에서 날라감
- `void Func(int\* num)`은 참조에 의한 호출로 `num의 주소를 복사`해온 것으로 `수정`하면 `num의 주소가 가리키는 값(원본)`을 변경함

- 참조는 별칭이다

  - ```cpp
    int number = 100;
    int& ref = number;
    ```

- 참조는 NULL이 될 수 없음

  - ```cpp
     int& ref = NULL; // error
    ```

- 참조는 초기화 중에 반드시 선언 되어야 함
  - ```cpp
    int& ref; // error
    ```
- 참조하는 대상을 바꿀 수 없다.
  - ```cpp
    int num1 = 100;
    int num2 = 100;
    int& ref = num1;
    ref = num2; // 세 변수가 모두 200이 됨
    ```

# std::string

- 문서 참조
- 사용법은 string쓰는거랑 비슷해서 딱히 볼 필요 없음
- `str.size()`, `str.length()` 차이 정도
- string을 생성하면 기본적으로 메모리를 힙에서 미리 할당받아둠
- 현재 메모리 사이즈보다 클 경우 새로 메모리를 할당 받고 기존 메모리를 memcpy로 복사해오고 해제 시킴, 그리고 string을 이어 붙임
- 위와 같은 문제 때문에 string보단 sprintf와 char[]를 많이 씀

# 파일입출력

- `endl`을 쓰면 flush()가 된다. `\n`은 flush()가 안됨.

# 개체지향

- 힙 사용은 느리고, 스택은 빠르다.
- 힙을 사용할 때 RAII를 잘 지킬 것.

  - RAII : "리소스 획득은 초기화", 개체가 리소스를 소유한다. 개체의 라이프사이클과 리소스 생성, 삭제를 똑같이 하자.
  - 가끔 RAII원칙을 어거야 할 경우가 있는데 팩토리패턴에서 볼 수 있다. 이땐 어쩔 수 없이 사용해야하니 관리를 잘하자.

# 개체 배열

- ```cpp
    // Vector는 int x,y 가지고 있음
    // 10개의 벡터 객체를 힙에 만듦 (80바이트)
    Vector* list = new Vector[10];

    // 10개의 포인터를 힙에 만듦 (40바이트)
    // 첫번째*는 배열, 두번째 *은 벡터의 포인터
    Vector** list = new Vector*[10];
  ```

# 개체 소멸

- `delete`, `delete[]`

# 초기화 리스트

```cpp
class A
{
  public:
      A() : mA(1), mB(2) // <- 초기화
      {
          mA = 1; // <- 대입
          mB = 2; // <- 대입
      }
  private:
      int mA;
      int mB;
}
```

초기화할 수 있는건 초기화 리스트로 하고 나머진 대입으로 할 것

# 생성자

- 클래스의 생성자가 없으면 기본생성자 만들어줌
- 생성자가 있으면 기본생성자 안만들어줌
- 생성자 오버로딩 가능

# 소멸자

- 클래스 지워질 때 자동 호출
- 동적할당한거 있으면 소멸자에서 지워야함
- 소멸자엔 delete만 하는게 일반적

# const 멤버 함수

`int GetX() const;`

- 해당 개체 안의 어떤 것도 바꾸지 않음
- 멤버 변수가 변하는 것을 방지
- 모든 함수는 const로 짜고 컴파일 에러나면 고치는게 좋다

# 구조체와 클래스

- 다른점은 기본이 public이냐 private이냐
- 어셈블리단에서는 클래스와 구조체를 구분하지 못한다
- 컴파일러단에서는 구분한다. 왜냐? public인지 private인지 구분해야하니까.
- C++에서 구조체 사용 규칙
  - C++에서 구조체는 순수한 데이터뿐 일때만 쓰자 (POD, Plain Old Data)
  - 사용자가 생성자, 소멸자 만들지 말 것 (제한적 허용)
  - private protected 쓰지 말 것
  - 가상 함수 쓰지 말 것
  - memcpy 가능함 (가상함수, 생성자 소멸자쓰면 불가능함)

# 복사 생성자

`ClassName(const ClassName& other);`

같은 클래스에 속한 다른 개체를 이용하여 새로운 개체를 초기화

```cpp
Vector(const Vector& other);

Vector a; // 기본 생성자 호출
Vector b(a); // 복사생상자 호출
```

- 복사생성자도 기본생성자처럼 암시적으로 만들어줌
- 암시적 복사생성자는 얕은 복사 사용
- 클래스에서 동적할당중이면 직접 복사 생성자를 작성해서 깊은 복사를 할 것

# 함수 오버로딩

```cpp
int Sum(int int);
int Sum(double double);

Sum(1, 3.14); // 컴파일 에러
```

- 모호하면 호출안됨

# 연산자 오버로딩

```cpp
// 둘이 같음
Vector sum = v1 + v2;
Vector sum = v1.operator+(v2);
```

- 대입연산자 operator=
  - 복사생성자와 동일
  - 대입연산자는 메모리를 해제할 필요가 있음
  - a = b일때 a의 값을 해제하고 b의 값을 memcpy해야함
  - 복사생성자를 구현했으면 대입도 구현해야함
  - 복사생성자와 마찬가지로 대입연산자로 암시적으로 만들어줌

# 클래스에 딸려오는 암시적 함수들

- 매개변수 없는 생성자
- 복사 생성자
- 소멸자
- 대입(=) 연산자
- 해당 함수들을 private에 넣으면 지울 수 있다.

# 상속

- `class Cat : public Animal {}`
- 상속 시 public만 쓰고 이외의 것은 잘 안씀
- 부모생성자 -> 자식생성자 -> 자식소멸자 -> 부모소멸자 순서

# 다형성

- 정적 바인딩
  - 무조건 선언한 변수의 자료형을 따라간다.
  ```cpp
    Animal* cat1 = new Cat();
    cat1.Speak(); // Animal
    Cat* cat2 = new Cat();
    cat2.Speak(); // Meow
  ```
- 동적 바인딩
  - 함수 앞에 virtual을 넣어 가상함수를 만들어준다.
  ```cpp
    class Animal
    {
    public:
        virtual void Speak();
    }
  ```
  - 다형성을 원한다면 `virutal`을 붙일 것

## 가상함수

- 부모클래스의 포인터 또는 참조를 사용하더라도 자식 클래스의 멤버함수가 언제든지 호출됨
- 동적 바인딩 = 늦은 바인딩
  - 실행중에 어떤 함수를 호출할지 결정해서 정적 바인딩보다 느릴 수 밖에 없음
- **가상 (함수) 테이블**이라는 것을 이용해서 어떤 가상 함수를 호출해야하는지 결정함
  - 함수의 주소를 저장해 놓는 테이블임, 테이블에 5개의 주소가 있으면 해당 클래스는 함수가 5개 있다는 뜻
- 개체를 생성할 때, 해당 클래스의 가상 테이블의 주소가 함께 저장됨.
- 가상 테이블은 룩업테이블 혹은 점프테이블이라고 불림

## 가상 소멸자

```cpp
class Animal
{
public:
    ~Animal();
private:
    int mAge;
}

class Cat : public Animal
{
public:
    ~Cat();
private:
    char* mName;
}

Cat::~Cat()
{
    delete mName;
}
```

위 코드에서 `Animal* cat = new Cat();` 후 삭제될 때 소멸자가 가상소멸자가 아니므로 Animal의 소멸자로 정적 바인딩 될 것이고 Cat의 소멸자가 호출이 안되기 때문에 이는 메모리 누수로 이어진다.

```cpp
class Animal
{
public:
    virtual ~Animal();
private:
    int mAge;
}

class Cat : public Animal
{
public:
    virtual ~Cat();
private:
    char* mName;
}
```

메모리 누수를 막기 위해선 부모클래스를 가상소멸자로 변경해주면 된다.

**메모리 누수 방지를 위해 모든 소멸자에 virtual 키워드를 넣을 것**

다형성은 여러 형태로 구현하는 것을 말한다.

# 다중 상속

- 다이아몬드 문제
- 다중 상속대신 인터페이스를 쓰자

# 추상 클래스 (abstract)

클래스의 함수가 하나라도 구현이 안되어있으면 추상클래스이다.

순수 가상함수를 가지고 있는 부모클래스를 추상클래스라고 한다.

```cpp
class Animal
{
public:
    virtual void Speak() = 0; // 순수가상함수
    virtual ~Animal();
}
```

- 순수 가상 함수: 구현체가 없는 멤버함수, 자식 클래스가 구현해야함 가상함수에 `= 0`붙이면됨

추상 클래스로 개체를 만들 수 없음  
추상 클래스를 포인터나 참조형으로 사용 가능함

```cpp
// Animal은 추상 클래스
Animal animal; // 컴파일 에러
Animal* animal = new Animal(); // 컴파일 에러
Animal* cat = new Cat();
Animal& catRef = *cat;
```

# 인터페이스

인터페이스란 멤버변수가 없고 순수가상함수만 있는 순수추상클래스를 말함.

# 캐스팅

## static_cast\<T\>

- 값
  - 두 숫자 형 간의 변환
    - 값을 유지
    - 이진수 표기는 달라질 수 있음
- 개체 포인터
  - 변수형 체크 후 부모클래스를 자식클래스로 변환

## reinterpret_cast\<T\>

재해석 캐스팅, C++ 캐스팅중 가장 위험한 캐스팅

- 연관 없는 두 포인터 형 사이의 변환 허용
- 포인터와 포인터 아닌 변수 사이의 형 변환 허용
- 이진수 표기는 달라지지 않음

```cpp
int* signedNumber = new int(-10); // -10
unsigned int* unsignedNumber = reinterpret_cast<unsigned int*>(signedNumber); // 4294967286
```

이진수는 변경되지 않았다.

## const_cast

const_cast은 형변환 불가능  
const나 volatile 애트리뷰트 제거하는 기능

## dynamic_cast

- 실행중에 형을 판단
- 포인터, 참조형 캐스팅시에만 사용 가능
- 호환되지 않은 자식형으로 캐스팅하면 NULL 반환
  - dynamic_cast가 static_cast보다 더 안전
- RTTI를 켜아 정상 작동, 아니면 static_cast와 똑같이 작동함
  - 대부분 C++ 프로젝트에선 RTTI 꺼놓음

# 캐스팅 규칙

1. static_cast만 쓸 것
2. static_cast가 에러를 뱉는다면 reinterpret_cast를 쓸 것
   - 단 그 데이터형이 100% 확신할 때만 사용할 것
3. 내가 변경권한이 없는 외부 라이브러리를 호출 할때만 const_cast를 쓸 것

# 인라인 함수

작은 함수들 짜잘하게 만들면 함수 오버헤드 때문에 성능에 문제가 생길 수 있음.  
이럴 때 inline키워드를 이용해 함수 오버헤드를 줄이면서 함수를 사용할 수 있음.  
inline키워드를 썼어도 컴파일러 상황에 따라 inline을 안해줄 수 있음.

`inline void Method();`

# static

- 정적 변수
  - 멤버 함수 안
  - 멤버 아닌 함수 안
- 정적 멤버 함수

범위(스코프)에 제한을 받는 전역 변수

- 범위

  - 파일 속
  - 네임스페이스 속
  - 클래스 속
    - 정적 멤버 변수
      - **클래스당 하나만 존재**
      - 개체의 메모리의 일부가 아니라 클래스 메모리에 포함됨
      - exe파일 안에 필요한 메모리가 잡혀있음
    - 정적 멤버 함수
      - 해당 클래스의 정적 멤버 변수에만 접근 가능
      - 개체가 없어도 함수 호출 가능
  - 함수 속
    - 함수 내에서 호출 시마다 초기화 되지 않고 계속 저장되는 변수

## 정적 멤버 변수 베스트 프랙티스

- 함수 속에 정적 변수 넣지 말 것
  - 클래스에 넣을 것
- 전역 변수 대신 정적 멤버 변수를 쓸 것
- C 스타일 정적 변수 사용 금지

# extern

- 다른 파일에 선언한 전역변수에 대한 접근을 가능하게 해줌

동작

- a.h에 extern 변수 선언
- main.cpp에서 `#include "a.h"`
- main.cpp에서 extern키워드가 있으니 있다고 가정하고 obj를 만듬, 이때 extern키워드가 붙어있는 애는 빈칸으로 놔둠
- a.h와 a.cpp도 obj를 만듬
- 링커에서 main obj와 a obj를 링크함
- main에서 a에서 extern의 존재를 알고 빈칸에 채워줌.

# 예외처리

- nullptr참조 시 예외안뜸
- stack overflow시 예외안뜸
- 0으로 나눌 때 예외안뜸

## c++ 예외

- 언어에서 만들어준게 아닌 프로그래머가 직접 만듬 (c++ 표준 라이브러리, `std::exception`)

- 언어에서 예외처리가 아니라 os단에서 시그널로 알려주는거다.

# STL

## vector

용량과 크기

```cpp
scores[3] = [10, 20]
// capacity = 3
// size = 2
```

- capacity()
  - vector에 할당된 요소 공간 수
- size()
  - vector에 실제 들어 있는 요소 수

용량 늘리기

- `reserve()` 불필요한 재할당을 막기 위해 vector 생성 직후 호출 할 것 (엄청 느림)

operator[]

- 지정된 위치의 요소를 **참조**로 반환

vector에 개체를 넣을 경우

- 재할당할 때 개체를 전부 복사해서 어마어마한 메모리를 사용한다. 포인터를 넣자

vector에 포인터를 넣을 경우

- `erase()`나 `clear()`를 한다고 delete가 호출 되지 않는다.
- iter로 delete 후 벡터에서 삭제하자.

## iterator (반복자)

```cpp
for (std::vector<int>::iterator iter = scores.begin(); iter != scores.end(); i++)
{
    std::cout << *iter << std::endl;
}
```

iterator는 개념상 포인터임  
첫번째 요소를 가리키고 플러스시키면 다음 요소 가리키고 반복

- `begin()`: 첫번째 요소를 가리키는 반복자 반환
- `end()`: 마지막 요소의 바로 뒤의 요소를 가리키는 반복자 반환
- 역방향으로 가고 싶으면 reverse iterator를 사용, `rbegin()`, `rend()`
- `clear()`는 size은 0이고 되고 용량은 변하지 않음

## map

insert()

- 반복자와 bool값을 한쌍으로 반환, 반복자는 요소, bool값은 삽입 결과 리턴
- 중복 삽입 불가

operator[]

- 대응하는 키가 없으면 새 요소 삽입
- 중복 될 시 덮어씀

key에 따라 자동 정렬이 된다

find()

- map에서 key를 찾으면 대응하는 반복자를 반환
- 없으면 end()를 반환

key로 개체를 넣을때

- 개체를 key로 넣고 싶으면 `operator<` 비교함수를 넣어줘야한다.
- 남이 만든 클래스여서 수정이 불가할땐 comparer를 이용해 넣어야한다

- ```cpp
    struct ClassInfoComparer
    {
        bool operator() (const ClassInfo& left, const ClassInfo& right) const
        {
            return (left.getName() < right.getName());
        }
    }
    std::map<ClassInfo, int, ClassInfoComparer> map;
  ```

map은 해쉬맵이 아님

## list, queue, stack

list, 링크드 리스트임

queue, stack은 아는거니까

## STL 컨테이너의 목적

- 모든 컨테이너에 적용되는 표준 인터페이스
- std 알고리듬은 많은 컨테이너에서 작동
- 템플릿 기반 프로그래밍
- 메모리 자동 관리
  - 메모리 단편화의 문제
    - reserve안하고 그냥 쓰면 내부에서 자동으로 관리해서 단편화됨
- EASTL이나 언리얼STL 찾아보기 (TArray, TMap...)
- 직접 컨테이너를 만들어보면 메모리에 관한 이해를 높일 수 있음
- STL보단 Array기반으로
  - 게임에선 최대 할당량을 정해놓고 쓰기때문에 자동 관리가 필요없고 귀찮아질 수 있음
