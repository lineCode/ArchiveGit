//
// Copyright (c) 1996, Stonehand Inc.  All rights reserved.
// 

#ifndef _H_REFCNT
#define _H_REFCNT
	   
  // a base class for reference counting

#ifdef _DEBUG
	void SCDebugBreak( void );
#endif

class RefCount {
public:
			RefCount() : refcnt_(0)
			{
#ifdef _DEBUG
				magic_ = 0xbabaabab;
#endif
			}
			RefCount(const RefCount &) : refcnt_(0)
			{
#ifdef _DEBUG
				magic_ = 0xbabaabab;
#endif
			}  
			
  virtual	~RefCount()
			  {
				  if ( refcnt_ )
					  throw( -1 );
			  }
		  
	
  int		decref()	  // return 1 if it should be deleted
			  {
#ifdef _DEBUG
				  static void* test = 0;
				  if ( this == test )
					  SCDebugBreak();
#endif				  
				  return --refcnt_ <= 0;
			  }

  void		incref()
			  {
#ifdef _DEBUG
				  static void* test = 0;
				  if ( this == test )
					  SCDebugBreak();
#endif
				  ++refcnt_;
			  }

  int		refcnt()
			  {
				  return refcnt_;
			  }
#ifdef _DEBUG
  unsigned	magic()
			  {
				return magic_;
			  }
#endif

private:
#ifdef _DEBUG
  unsigned	magic_;
#endif
  int		refcnt_;
};


//////////////////////////////////////////////////////////////
// The following are classes to maintain safe reference count on
// classes derived from RefCount

// T must have RefCount as a public base class
// T may be an incomplete type

template<class T>
class ConstRefCountPtr {
public:
		ConstRefCountPtr() : p_(0) { }
		ConstRefCountPtr( T* p ) : p_( p )
			{
				if ( p_ )
					p_->incref();
			}	
			  
		~ConstRefCountPtr()
			{
				clear();
			}		  
		ConstRefCountPtr( const ConstRefCountPtr<T>& p ) :
			  p_(p.p_)
			{
			  if (p.p_)
				  p.p_->incref();
			}			  
		ConstRefCountPtr<T> &operator=( const ConstRefCountPtr<T> & p )
			{
			  if ( this != &p ) {
				  if (p.p_)
					  p.p_->incref();

				  clear();

				  p_ = p.p_;
			  }
			  return *this;
			}
									

	void	moveTo( ConstRefCountPtr<T> &dst )
			  {
				  if ( this == &dst )
					  return;

				  dst.clear();
				  
				  dst.p_ = p_;
				  p_ = 0;
			  }
			  
	void	clear()
			  {
				  if ( p_ && p_->decref())
					  delete p_;
				  p_ = 0;
			  }
  
	const T*  ptr() const
			  {
				  return p_;
			  }
	const T*  operator->() const
			  {
				  return p_;
			  }
	const T&  operator*() const
			  {
				  return *p_;
			  }
	int isNull() const
			  {
				  return p_ == 0;
			  }
	int operator!() const
			  {
				return p_ == 0;
			  }
	int operator==( const ConstRefCountPtr<T>& p ) const
			  {
				  return p_ == p.p_;
			  }
	int operator!=( const ConstRefCountPtr<T>& p ) const
			  {
				  return p_ != p.p_;
			  }
	int operator==( const T* p ) const
			  {
				  return p_ == p;
			  }
	int operator!=( const T* p ) const
			  {
				  return p_ != p;
			  }
  
protected:
	T*	p_;
};


template<class T>
class RefCountPtr : public ConstRefCountPtr<T> {
public:
	  RefCountPtr( T* ptr = 0 ) :
		ConstRefCountPtr<T>( ptr ){ }
		
	  ~RefCountPtr() { }
  T*	ptr() const
		{
		  return p_;
		}
  T*	operator->() const
		{
		  return p_;
		}
  T&	operator*() const
		{
		  return *p_;
		}
  void	exch(RefCountPtr<T> &p)
		{
		  T* tmp = p.p_;
		  p.p_ = p_;
		  p_ = tmp;
		}
};

#endif
