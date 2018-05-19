<?hh // strict
namespace Usox\Sharesta;

class ResponseTest extends \PHPUnit_Framework_TestCase {

	public function testSend500InvalidHttpStatus(): void {
		$response = new Response(666,' AGGIAGGI');

		$this->expectOutputString(
			'"API attempted to return an unknown HTTP status."',
		);
		$response->send();
	}

	public function testSendReturnsDefaultBodyFor200(): void {
		$response = new Response(200, null);

		$this->expectOutputString(
			'"OK"',
		);
		$response->send();
	}

	public function testSendReturnsBodyWith200(): void {
		$body = 'my-body';
		$response = new Response(200, $body);

		$this->expectOutputString(
			$body
		);
		$response->send();
	}
}
