<?hh // strict
namespace Usox\Sharesta;

use function Facebook\FBExpect\expect;

class RequestTest extends \PHPUnit_Framework_TestCase {

	public function testRequestEstablishesPropertiesOnCreation(): void {
		// UNSAFE
		$_SERVER['REQUEST_URI'] = '/api/test/sub/1';
		$_SERVER['REQUEST_METHOD'] = 'GET';
		$_GET = array('a' => '1', 'b' => '2');

		$request = new Request(
			dict($_SERVER),
			dict($_GET),
			new RequestBody()
		);
		$uri_values = $request->getUriValues();

		expect($request->getRoute('/api/'))->toBeSame('test/sub/1');
		expect($request->getHttpMethod())->toBeSame('GET');
		expect($uri_values['b'])->toBeSame('2');
	}
}
