package err

type ErrTimeout struct{}
type ErrCanceled struct{}
type ErrEmptied struct{}
type ErrInvalidParameter struct{}

func (e ErrTimeout) Error() string {
	return "timeout"
}

func (e ErrCanceled) Error() string {
	return "canceled"
}

func (e ErrEmptied) Error() string {
	return "emptied"
}

func (e ErrInvalidParameter) Error() string {
	return "invalid paramater"
}

var Timeout *ErrTimeout = &ErrTimeout{}
var Canceled *ErrCanceled = &ErrCanceled{}
var Emptied *ErrEmptied = &ErrEmptied{}
var InvalidParameter *ErrInvalidParameter = &ErrInvalidParameter{}
