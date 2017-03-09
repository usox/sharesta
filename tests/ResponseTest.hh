<?hh // partial
namespace Usox\Sharesta;

use Mockery as m;

function header($text) {
	ResponseTest::$functions->header($text);
}

class ResponseTest extends \PHPUnit_Framework_TestCase {

	public static $functions = null;

	public function setUp() {
		static::$functions = m::mock();

		static::$functions->shouldReceive('header')
			->with('Content-type: application/json')
			->once();
	}

	public function testSend500InvalidHttpStatus(): void {
		$response = new Response(666,' AGGIAGGI');

		static::$functions->shouldReceive('header')
			->with('HTTP/1.1 500 Internal Server Error')
			->once();

		$this->expectOutputString(
			'"API attempted to return an unknown HTTP status."',
		);
		$response->send();
	}

	public function testSendReturnsDefaultBodyFor200() {
		$response = new Response(200, null);

		static::$functions->shouldReceive('header')
			->with('HTTP/1.1 200 OK')
			->once();

		$this->expectOutputString(
			'"OK"',
		);
		$response->send();
	}

	public function testSendReturnsBodyWith200() {
		$body = 'my-body';
		$response = new Response(200, $body);

		static::$functions->shouldReceive('header')
			->with('HTTP/1.1 200 OK')
			->once();

		$this->expectOutputString(
			sprintf('"%s"', $body)
		);
		$response->send();
	}
}
