<?hh // partial
namespace Usox\Sharesta;

function header($text) {
	ResponseTest::$functions->header($text);
}

class ResponseTest extends \PHPUnit_Framework_TestCase {

	public static $functions = null;

	public function setUp() {
		static::$functions = $this
			->getMockBuilder(\stdClass::class)
			->setMethods([
				'header'
			])
			->getMock();
	}

	public function testSend500InvalidHttpStatus(): void {
		$this->markTestSkipped('Skipped due to hhvm issues with global namespace (header method)');
		$response = new Response(666,' AGGIAGGI');

		static::$functions
			->expects(static::at(0))
			->method('header')
			->with('HTTP/1.1 500 Internal Server Error');
		static::$functions
			->expects(static::at(1))
			->method('header')
			->with('Content-type: application/json');

		$this->expectOutputString(
			'"API attempted to return an unknown HTTP status."',
		);
		$response->send();
	}

	public function testSendReturnsDefaultBodyFor200() {
		$this->markTestSkipped('Skipped due to hhvm issues with global namespace (header method)');
		$response = new Response(200, null);

		static::$functions
			->expects(static::at(0))
			->method('header')
			->with('HTTP/1.1 200 OK');
		static::$functions
			->expects(static::at(1))
			->method('header')
			->with('Content-type: application/json');

		$this->expectOutputString(
			'"OK"',
		);
		$response->send();
	}

	public function testSendReturnsBodyWith200() {
		$this->markTestSkipped('Skipped due to hhvm issues with global namespace (header method)');
		$body = 'my-body';
		$response = new Response(200, $body);

		static::$functions
			->expects(static::at(0))
			->method('header')
			->with('HTTP/1.1 200 OK');
		static::$functions
			->expects(static::at(1))
			->method('header')
			->with('Content-type: application/json');

		$this->expectOutputString(
			$body
		);
		$response->send();
	}
}
